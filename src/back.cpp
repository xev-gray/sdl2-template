#include <back.hpp>


std::shared_ptr<Back> Back::instance = nullptr;

Input::Input()
{
	this -> spritesheet = nullptr;
	wasPressed = false;
	instantiated = true;
}

Input::Input(const std::shared_ptr<Spritesheet> spritesheet)
{
	if(spritesheet == nullptr)
	{
		SDL_Log("Cannot create Spritesheet: Spritesheet is null\n");
		instantiated = false;
	}
	else
	{
		this -> spritesheet = std::shared_ptr<Spritesheet>(spritesheet);
		wasPressed = false;
		instantiated = true;
	}
}



Click::Click(const Uint32 click) : Input()
{
	if(click == 0)
	{
		SDL_Log("Cannot create Click: Invalid click value\n");
		instantiated = false;
	}
	else
	{
		this -> click = click;
		wasHovered = false;
		instantiated = true;
	}
}

Click::Click(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet)
: Input(spritesheet)
{
	if(click == 0)
	{
		SDL_Log("Cannot create Click: Invalid click value\n");
		instantiated = false;
	}
	else if(spritesheet == nullptr)
	{
		SDL_Log("Cannot create Click: Spritesheet is null\n");
		instantiated = false;
	}
	else
	{
		this -> click = click;
		this -> spritesheet = spritesheet;
		wasHovered = false;
		instantiated = true;
	}
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
	if(value == 0)
	{
		SDL_Log("Cannot create Key: Invalid key value\n");
		instantiated = false;
	}
	else
	{
		this -> value = value;
		instantiated = true;
	}
}

Key::Key(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet)
: Input(spritesheet)
{
	if(value == 0)
	{
		SDL_Log("Cannot create Key: Invalid key value\n");
		instantiated = false;
	}
	else if(spritesheet == nullptr)
	{
		SDL_Log("Cannot create Key: Spritesheet is null\n");
		instantiated = false;
	}
	else
	{
		this -> value = value;
		this -> spritesheet = spritesheet;
		instantiated = true;

	}
}

Uint8 Key::pressed(const Uint8* keys)
{
	if(keys == nullptr)
		SDL_Log("Cannot update Key: Keystates array is null\n");
	else
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
	}

	return 0;
}



Back::Back()
{
	instantiated = true;
}

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
	if(value == 0)
		SDL_Log("Cannot add Key: Invalid key value\n");
	else
	{
		std::shared_ptr<Key> key = std::make_shared<Key>(value);
		if(key -> isInstantiated())
			keys.push_back(key);
	}
}

void Back::addKey(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet)
{
	if(value == 0)
		SDL_Log("Cannot add Key: Invalid key value\n");
	else if(spritesheet == nullptr)
		SDL_Log("Cannot add Key: spritesheet is null\n");
	else
	{
		std::shared_ptr<Key> key = std::make_shared<Key>(value, spritesheet);
		if(key -> isInstantiated())
			keys.push_back(key);
	}
}

void Back::addClick(const Uint32 click)
{
	if(click == 0)
		SDL_Log("Cannot add Click: Invalid click value\n");
	else
	{
		std::shared_ptr<Click> clk = std::make_shared<Click>(click);
		if(clk -> isInstantiated())
			clicks.push_back(clk);
	}
}

void Back::addClick(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet)
{
	if(click == 0)
		SDL_Log("Cannot add Click: Invalid click value\n");
	else if(spritesheet == nullptr)
		SDL_Log("Cannot add Click: Spritesheet is null\n");
	else
	{
		std::shared_ptr<Click> clk = std::make_shared<Click>(click, spritesheet);
		if(clk -> isInstantiated())
			clicks.push_back(clk);
	}
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
	if(handle == nullptr)
	{
		SDL_Log("Cannot update back-end: Handle function is null\n");
		return EXIT;
	}
	else if(front == nullptr)
	{
		SDL_Log("Cannot update back-end: Front-end is null\n");
		return EXIT;
	}
	else
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
}
