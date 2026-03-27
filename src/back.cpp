#include <back.hpp>


std::shared_ptr<inputs_t> UserInputs::userInputs = nullptr;

std::shared_ptr<Back> Back::instance = nullptr;

SpritesAddon::SpritesAddon() {}

void SpritesAddon::setSpritesheet(const std::shared_ptr<Spritesheet> newSpritesheet)
{
	if(newSpritesheet == nullptr)
		SDL_Log("Cannot set new Spritesheet: Spritesheet is null\n");
	else
		spritesheet = newSpritesheet;
}



SfxAddon::SfxAddon() : chunk(nullptr) {}

void SfxAddon::setSfx(const std::shared_ptr<Chunk> newChunk)
{
	if(newChunk == nullptr)
		SDL_Log("Cannot set new Sfx: Chunk is null\n");
	else
		chunk = newChunk;
}



template<typename... Addons>
Input<Addons...>::Input()
{
	wasPressed = false;
	instantiated = true;
}



const std::shared_ptr<inputs_t> UserInputs::get()
{
	return userInputs;
}

void UserInputs::set(std::shared_ptr<inputs_t> newUserInputs)
{
	userInputs = newUserInputs;
}

template<typename... Addons>
Click<Addons...>::Click(const Uint32 click) : Input<Addons...>()
{
	if(click == 0)
		SDL_Log("Cannot create Click: Invalid click value\n");
	else
	{
		this -> click = click;
		wasHovered = false;
		this -> instantiated = true;
	}
}

template<typename... Addons>
Uint32 Click<Addons...>::pressed()
{
	/* Manipulation of the spritesheet and/or the SFX
	 * only if it exists
	 */
	std::shared_ptr<Spritesheet> spritesheet = nullptr;
	std::shared_ptr<Chunk> chunk = nullptr;

	if constexpr(sizeof... (Addons) == 2)
	{
		spritesheet = this -> spritesheet;
		chunk = this -> chunk;
	}
	else if constexpr(sizeof... (Addons) == 1)
	{
		if constexpr(std::is_same_v<std::tuple_element_t<0, std::tuple<Addons...>>, SpritesAddon>)
			spritesheet = this -> spritesheet;
		else
			chunk = this -> chunk;
	}

	/* Boolean used for playing the SFX only once */
	static bool wasPlayed = false;

	if(spritesheet == nullptr)
	{
		Uint32 isClicked = UserInputs::get() -> mouseClicks & SDL_BUTTON(click);
		if(chunk != nullptr)
		{
			if(isClicked && !wasPlayed)
			{
				if(chunk -> isPlaying())
					chunk -> stop();
				chunk -> play();
				wasPlayed = true;
			}
			else if(!isClicked)
				wasPlayed = false;
		}

		return isClicked;
	}
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

		mouseX = scaleX * UserInputs::get() -> mouseX;
		mouseY = scaleY * UserInputs::get() -> mouseY;

		/* Processing mouse position and click */
		SDL_Rect dst = spritesheet -> getDst();
		if((UserInputs::get() -> mouseClicks) & SDL_BUTTON(click))
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
				if(chunk != nullptr && !wasPlayed)
				{
					if(chunk -> isPlaying())
						chunk -> stop();
					chunk -> play();
					wasPlayed = true;
				}

				this -> wasPressed = true;
			}
			else
			{
				if(spritesheet -> getIndex() != CLICK_DEFAULT)
					spritesheet -> setIndex(CLICK_DEFAULT);

				this -> wasPressed = false;
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

				if(this -> wasPressed)
				{
					if(chunk != nullptr)
						wasPlayed = false;
					this -> wasPressed = false; // This line is needed to activate the button only once per click
					return click;
				}
				else
					wasHovered = true;
			}
			else
			{
				if(spritesheet -> getIndex() != CLICK_DEFAULT)
					spritesheet -> setIndex(CLICK_DEFAULT);
				if(chunk != nullptr)
					wasPlayed = false;

				wasHovered = false;
			}

			this -> wasPressed = false;
		}

		return 0;
	}
}



template<typename... Addons>
Key<Addons...>::Key(const Uint8 value) : Input<Addons...>()
{
	if(value == 0)
		SDL_Log("Cannot create Key: Invalid key value\n");
	else
	{
		this -> value = value;
		this -> instantiated = true;
	}
}

template<typename... Addons>
Uint32 Key<Addons...>::pressed()
{
	static const Uint8* keys = UserInputs::get() -> keys;
	if(keys == nullptr)
		SDL_Log("Cannot update Key: Keystates array is null\n");
	else
	{
		/* Manipulation of the spritesheet and/or the SFX
		 * only if it exists
		 */
		std::shared_ptr<Spritesheet> spritesheet = nullptr;
		std::shared_ptr<Chunk> chunk = nullptr;
		if constexpr(sizeof... (Addons) == 2)
		{
			spritesheet = this -> spritesheet;
			chunk = this -> chunk;
		}
		else if constexpr(sizeof... (Addons) == 1)
		{
			if constexpr(std::is_same_v<std::tuple_element_t<0, std::tuple<Addons...>>, SpritesAddon>)
				spritesheet = this -> spritesheet;
			else
				chunk = this -> chunk;
		}

		/* Boolean used for playing the SFX only once */
		static bool wasPlayed = false;

		if(spritesheet == nullptr)
		{
			if(keys[value])
			{
				if(chunk != nullptr && !wasPlayed)
				{
					if(chunk -> isPlaying())
						chunk -> stop();
					chunk -> play();
					wasPlayed = true;
				}
				return value;
			}
			else
			{
				if(chunk != nullptr)
					wasPlayed = false;
				return 0;
			}
		}
		else
		{
			if(keys[value])
			{
				if(spritesheet -> getIndex() != KEY_PRESSED)
					spritesheet -> setIndex(KEY_PRESSED);
				if(chunk != nullptr && !wasPlayed)
				{
					if(chunk -> isPlaying())
						chunk -> stop();
					chunk -> play();
					wasPlayed = true;
				}

				this -> wasPressed = true;
			}
			else
			{
				if(spritesheet -> getIndex() != KEY_DEFAULT)
					spritesheet -> setIndex(KEY_DEFAULT);

				if(this -> wasPressed)
				{
					if(chunk != nullptr)
						wasPlayed = false;
					this -> wasPressed = false; // This line is needed to activate the button only once per click
					return value;
				}

				this -> wasPressed = false;
			}
		}
	}

	return 0;
}



Back::Back()
{
	instantiated = true;
	UserInputs::set(std::make_shared<inputs_t>());
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
		std::shared_ptr<Key<>> key = std::make_shared<Key<>>(value);
		if(key -> isInstantiated())
			keys.push_back(key);
	}
}

void Back::addKey(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet)
{
	if(value == 0)
		SDL_Log("Cannot add Key: Invalid key value\n");
	else if(spritesheet == nullptr)
		SDL_Log("Cannot add Key: Spritesheet is null\n");
	else
	{
		std::shared_ptr<Key<SpritesAddon>> key = std::make_shared<Key<SpritesAddon>>(value);
		if(key -> isInstantiated())
		{
			key -> setSpritesheet(spritesheet);
			keys.push_back(key);
		}
	}
}

void Back::addKey(const Uint8 value, const std::shared_ptr<Chunk> chunk)
{
	if(value == 0)
		SDL_Log("Cannot add Key: Invalid key value\n");
	else if(chunk == nullptr)
		SDL_Log("Cannot add Key: Chunk is null\n");
	else
	{
		std::shared_ptr<Key<SfxAddon>> key = std::make_shared<Key<SfxAddon>>(value);
		if(key -> isInstantiated())
		{
			key -> setSfx(chunk);
			keys.push_back(key);
		}
	}
}

void Back::addKey(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet, const std::shared_ptr<Chunk> chunk)
{
	if(value == 0)
		SDL_Log("Cannot add Key: Invalid key value\n");
	else if(spritesheet == nullptr)
		SDL_Log("Cannot add Key: Spritesheet is null\n");
	else if(chunk == nullptr)
		SDL_Log("Cannot add Key: Chunk is null\n");
	else
	{
		std::shared_ptr<Key<SpritesAddon, SfxAddon>> key = std::make_shared<Key<SpritesAddon, SfxAddon>>(value);
		if(key -> isInstantiated())
		{
			key -> setSpritesheet(spritesheet);
			key -> setSfx(chunk);
			keys.push_back(key);
		}
	}
}

void Back::addClick(const Uint32 click)
{
	if(click == 0)
		SDL_Log("Cannot add Click: Invalid click value\n");
	else
	{
		std::shared_ptr<Click<>> clk = std::make_shared<Click<>>(click);
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
		std::shared_ptr<Click<SpritesAddon>> clk = std::make_shared<Click<SpritesAddon>>(click);
		if(clk -> isInstantiated())
		{
			clk -> setSpritesheet(spritesheet);
			clicks.push_back(clk);
		}
	}
}

void Back::addClick(const Uint32 click, const std::shared_ptr<Chunk> chunk)
{
	if(click == 0)
		SDL_Log("Cannot add Click: Invalid click value\n");
	else if(chunk == nullptr)
		SDL_Log("Cannot add Click: Chunk is null\n");
	else
	{
		std::shared_ptr<Click<SfxAddon>> clk = std::make_shared<Click<SfxAddon>>(click);
		if(clk -> isInstantiated())
		{
			clk -> setSfx(chunk);
			clicks.push_back(clk);
		}
	}
}

void Back::addClick(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet, const std::shared_ptr<Chunk> chunk)
{
	if(click == 0)
		SDL_Log("Cannot add Click: Invalid click value\n");
	else if(spritesheet == nullptr)
		SDL_Log("Cannot add Click: Spritesheet is null\n");
	else if(chunk == nullptr)
		SDL_Log("Cannot add Click: Chunk is null\n");
	else
	{
		std::shared_ptr<Click<SpritesAddon, SfxAddon>> clk = std::make_shared<Click<SpritesAddon, SfxAddon>>(click);
		if(clk -> isInstantiated())
		{
			clk -> setSpritesheet(spritesheet);
			clk -> setSfx(chunk);
			clicks.push_back(clk);
		}
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

	while(SDL_PollEvent(&(UserInputs::get() -> event)))
		shouldQuit = UserInputs::get() -> event.type != SDL_QUIT;

	return shouldQuit;
}

void Back::updateKeys()
{
	UserInputs::get() -> keys = SDL_GetKeyboardState(nullptr);
}

void Back::updateMouse()
{
	UserInputs::get() -> mouseClicks = SDL_GetMouseState(&(UserInputs::get() -> mouseX), &(UserInputs::get() -> mouseY));
}

const std::vector<std::shared_ptr<Pressable>>& Back::getKeys() const
{
	return keys;
}

const std::vector<std::shared_ptr<Pressable>>& Back::getClicks() const
{
	return clicks;
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
