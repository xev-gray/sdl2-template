#include "back.hpp"


std::shared_ptr<Back> Back::instance = nullptr;

Input::Input()
{
	this -> spritesheet = nullptr;
	wasPressed = false;
}

Input::Input(const char* const path)
{
	if(path == nullptr)
		SDL_Log("spritesheet path is null\n");
	else
		this -> spritesheet = std::shared_ptr<Spritesheet>(new Spritesheet(path));
	wasPressed = false;
}

Input::Input(const std::shared_ptr<Spritesheet> spritesheet)
{
	if(spritesheet == nullptr)
		SDL_Log("spritesheet is null\n");
	else
		this -> spritesheet = std::shared_ptr<Spritesheet>(spritesheet);
	wasPressed = false;
}



Click::Click(const Uint32 click) : Input()
{
	this -> click = click;
	wasHovered = false;
}

Click::Click(const Uint32 click, const char* const path) : Input (path)
{
	this -> click = click;
	this -> spritesheet = std::make_shared<Spritesheet>(path);
	wasHovered = false;
}

Click::Click(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet)
: Input(spritesheet)
{
	this -> click = click;
	this -> spritesheet = spritesheet;
	wasHovered = false;
}

Uint32 Click::pressed(const inputs_t& userInputs)
{
	if(spritesheet == nullptr)
		return userInputs.mouseClicks & SDL_BUTTON(click);
	else
	{
		/* Responsive mouse position capture */
		SDL_Window* window = SDL_RenderGetWindow(Renderer::get());
		int windowW, windowH;
		int mouseX, mouseY;

		SDL_GetWindowSize(window, &windowW, &windowH);

		float scaleX = APP_W;
		float scaleY = APP_H;

		scaleX /= windowW;
		scaleY /= windowH;

		mouseX = scaleX * userInputs.mouseX;
		mouseY = scaleY * userInputs.mouseY;

		/* Processing mouse position and click */
		SDL_Rect dst = spritesheet -> getDst();
		if(userInputs.mouseClicks & SDL_BUTTON(click))
		{
			/* Added 'SDL_GetMouseFocus() == window' for when
			 * the clickable button is at the corner of the
			 * window and the mouse hovers the button before
			 * changing focus, which stops it from being
			 * captured and therefore leaves the button on
			 * CLICK_HOVERED
			 */
			if(mouseX >= dst.x && mouseY >= dst.y
			&& mouseX < dst.x + dst.w && mouseY < dst.y + dst.h
			&& mouseX >= 0 && mouseY >= 0
			&& mouseX < APP_W && mouseY < APP_H
			&& SDL_GetMouseFocus() == window && wasHovered)
			{
				if(spritesheet -> getIndex() != CLICK_PRESSED)
					spritesheet -> setIndex(CLICK_PRESSED);
				wasPressed = true;
			}
			else
			{
				if(spritesheet -> getIndex() != CLICK_DEFAULT)
					spritesheet -> setIndex(CLICK_DEFAULT);

				wasPressed = false;
				wasHovered = false;
			}
		}
		else
		{
			if(mouseX >= dst.x && mouseY >= dst.y
			&& mouseX < dst.x + dst.w && mouseY < dst.y + dst.h
			&& mouseX >= 0 && mouseY >= 0
			&& mouseX < APP_W && mouseY < APP_H
			&& SDL_GetMouseFocus() == window)
			{
				if(spritesheet -> getIndex() != CLICK_HOVERED)
					spritesheet -> setIndex(CLICK_HOVERED);

				if(wasPressed)
				{
					wasPressed = false; // This line is needed to activate the button only once per click
					return click;
				}
				else
					wasHovered = true;
			}
			else
			{
				if(spritesheet -> getIndex() != CLICK_DEFAULT)
					spritesheet -> setIndex(CLICK_DEFAULT);
				wasHovered = false;
			}

			wasPressed = false;
		}

		return 0;
	}
}



Key::Key(const Uint8 value) : Input()
{
	this -> value = value;
}

Key::Key(const Uint8 value, const char* const path) : Input (path)
{
	this -> value = value;
	this -> spritesheet = std::make_shared<Spritesheet>(path);
}

Key::Key(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet)
: Input(spritesheet)
{
	this -> value = value;
	this -> spritesheet = spritesheet;
}

Uint8 Key::pressed(const Uint8* keys)
{
	if(keys[value])
	{
		if(spritesheet != nullptr && spritesheet -> getIndex() != KEY_PRESSED)
			spritesheet -> setIndex(KEY_PRESSED);

		wasPressed = true;
	}
	else
	{
		if(spritesheet != nullptr && spritesheet -> getIndex() != KEY_DEFAULT)
			spritesheet -> setIndex(KEY_DEFAULT);

		if(wasPressed)
		{
			wasPressed = false; // This line is needed to activate the button only once per click
			return value;
		}

		wasPressed = false;
	}

	return 0;
}



Back::Back() {}

/* Singleton: for one App there should only be
 * one back-end object
 */
std::shared_ptr<Back> Back::getInstance()
{
	if(instance == nullptr)
		instance = std::shared_ptr<Back> (new Back());
	return instance;
}

void Back::addKey(const Uint8 value)
{
	keys.push_back(std::shared_ptr<Key> (new Key(value)));
}

void Back::addKey(const Uint8 value, const char* const path)
{
	if(path == nullptr)
		SDL_Log("spritesheet path is null\n");
	else
		keys.push_back(std::shared_ptr<Key> (new Key(value, path)));
}

void Back::addKey(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet)
{
	if(spritesheet == nullptr)
		SDL_Log("spritesheet is null\n");
	else
		keys.push_back(std::shared_ptr<Key> (new Key(value, spritesheet)));
}

void Back::addClick(const Uint32 click)
{
	clicks.push_back(std::shared_ptr<Click> (new Click(click)));
}

void Back::addClick(const Uint32 click, const char* const path)
{
	if(path == nullptr)
		SDL_Log("spritesheet path is null\n");
	else
		clicks.push_back(std::shared_ptr<Click> (new Click(click, path)));
}

void Back::addClick(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet)
{
	if(spritesheet == nullptr)
		SDL_Log("spritesheet is null\n");
	else
		clicks.push_back(std::shared_ptr<Click> (new Click(click, spritesheet)));
}

/* Write your own Entity adder here */
void Back::addEntity(/* ... */)
{
	// ...
}

bool Back::updateEvent()
{
	bool shouldQuit = true;

	while(SDL_PollEvent(&(userInputs.event)))
		shouldQuit = userInputs.event.type != SDL_QUIT;

	return shouldQuit;
}

void Back::updateKeys()
{
	userInputs.keys = SDL_GetKeyboardState(nullptr);
}

void Back::updateMouse()
{
	userInputs.mouseClicks = SDL_GetMouseState(&(userInputs.mouseX), &(userInputs.mouseY));
}

const std::vector<std::shared_ptr<Key>>& Back::getKeys() const
{
	return keys;
}

const std::vector<std::shared_ptr<Click>>& Back::getClicks() const
{
	return clicks;
}

const inputs_t& Back::getUserInputs() const
{
	return userInputs;
}

void Back::clearInputs()
{
	keys.clear();
	clicks.clear();
}

/* You can change this method to clear a vector
 * of a child class of Entity instead
 */
void Back::clearEntities()
{
	entities.clear();
}

/* The front-end call looks messy but I had no
 * other choice to make it work
 */
Sint32 Back::update(const std::function<Sint32(std::shared_ptr<Front>, std::shared_ptr<Back>)> handle,
					const std::shared_ptr<Front> front)
{
	if(!updateEvent())
		return EXIT;
	else
	{
		updateKeys();
		updateMouse();
		return handle(front, instance);
	}
}
