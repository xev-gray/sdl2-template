#include <app.hpp>


std::shared_ptr<App> App::instance = nullptr;

App::App()
{
	init_flags = INIT_SDL;

	/* Remove the flags you don't need */
	Uint32 sdl_flags = SDL_INIT_TIMER
	                 | SDL_INIT_AUDIO
	                 | SDL_INIT_VIDEO
	                 | SDL_INIT_JOYSTICK
	                 | SDL_INIT_HAPTIC
	                 | SDL_INIT_GAMECONTROLLER
	                 | SDL_INIT_EVENTS
	;
	if(SDL_Init(sdl_flags) != 0)
	{
		init_flags = INIT_FAIL;
		SDL_Log("%s\n", SDL_GetError());
		instantiated = false;
	}
	else
		instantiated = true;
}

/* Init flags are defined in the 'common.hpp' header */
App::App(const Uint8 flags)
{
	if(flags == 0 || flags == INIT_FAIL)
	{
		SDL_Log("Cannot create App: Invalid App flags\n");
		instantiated = false;
	}
	else
	{
		init_flags = flags | INIT_SDL;

		/* Remove the flags you don't need */
		Uint32 sdl_flags = SDL_INIT_TIMER
		                 | SDL_INIT_AUDIO
		                 | SDL_INIT_VIDEO
		                 | SDL_INIT_JOYSTICK
		                 | SDL_INIT_HAPTIC
		                 | SDL_INIT_GAMECONTROLLER
		                 | SDL_INIT_EVENTS
		;
		if(SDL_Init(sdl_flags) != 0)
		{
			init_flags = INIT_FAIL;
			SDL_Log("%s\n", SDL_GetError());
			instantiated = false;
		}

		if(flags & INIT_IMAGE)
		{
			/* Remove the flags you don't need */
			Uint8 img_flags = IMG_INIT_JPG
			                | IMG_INIT_PNG
			                | IMG_INIT_TIF
			                | IMG_INIT_WEBP
//			                | IMG_INIT_JXL
//			                | IMG_INIT_AVIF
			;
			if(IMG_Init(img_flags) != img_flags)
			{
				init_flags = INIT_FAIL;
				SDL_Log("%s\n", IMG_GetError());
				instantiated = false;
			}
		}

		if((flags & INIT_TTF) && (TTF_Init() != 0))
		{
			init_flags = INIT_FAIL;
			SDL_Log("%s\n", TTF_GetError());
			instantiated = false;
		}

		if(flags & INIT_MIXER)
		{
			/* Remove the flags you don't need */
			int mix_flags = MIX_INIT_FLAC
			              | MIX_INIT_MOD
			              | MIX_INIT_MP3
			              | MIX_INIT_OGG
//			              | MIX_INIT_MID // NOTE: This flag causes a memory leak.
			              | MIX_INIT_OPUS
//			              | MIX_INIT_musPACK
			;
			if(Mix_Init(mix_flags) != mix_flags)
			{
				init_flags = INIT_FAIL;
				SDL_Log("%s\n", Mix_GetError());
				instantiated = false;
			}
		}

		if((flags & INIT_NET) && (SDLNet_Init() != 0))
		{
			init_flags = INIT_FAIL;
			SDL_Log("%s\n", SDLNet_GetError());
			instantiated = false;
		}

		if(init_flags != INIT_FAIL)
			instantiated = true;
	}
}

/* There must be only one app for each execution */
std::shared_ptr<App> App::getInstance()
{
	if(instance == nullptr)
		instance = std::shared_ptr<App> (new App());
	return instance;
}

std::shared_ptr<App> App::getInstance(const Uint8 flags)
{
	if(flags == 0 || flags == INIT_FAIL)
	{
		SDL_Log("Cannot get App: Invalid App flags\n");
		return nullptr;
	}
	else
	{
		if(instance == nullptr)
			instance = std::shared_ptr<App> (new App(flags));
		return instance;
	}
}

App::~App()
{
	SDLNet_Quit();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool App::initFront(const char* const windowName)
{
	if(windowName == nullptr)
	{
		SDL_Log("Cannot create window: Window name is null\n");
		return false;
	}
	front = Front::getInstance(windowName);
	return front -> isInstantiated();
}

bool App::initBack()
{
	back = Back::getInstance();
	return back -> isInstantiated();
}

/* TODO: Add a loading screen before init calls */
Sint32 App::run(const std::function<void(std::shared_ptr<Front>, std::shared_ptr<Back>)> init,
				const std::function<Sint32(std::shared_ptr<Front>, std::shared_ptr<Back>)> handle,
				const char* const bgm)
{
	init(front, back);

	Sint32 result = PASS;
	bool works = true;

	std::shared_ptr<Music> music = nullptr;
	if(init_flags & INIT_MIXER)
	{
		Audio::open();
		if(bgm != nullptr)
		{
			music = Music::getInstance(bgm);
			if(music != nullptr && music -> isInstantiated())
				music -> play();
		}
	}

	while(works && result == PASS)
	{
		result = back -> update(handle, front);
		works = front -> render();

		SDL_Delay(static_cast<int>(1.0 / static_cast<float>(fps) * 1000.0));
	}

	if(music != nullptr)
	{
		if(music -> isPlaying() || music -> isPaused())
			music -> stop();
		Audio::close();
	}

	front -> clearViews();
	back -> clearInputs();
	back -> clearEntities();

	return result;
}
