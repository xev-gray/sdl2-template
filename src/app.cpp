#include "app.hpp"


std::shared_ptr<App> App::instance = nullptr;

App::App()
{
	/* Remove the flags you don't need */
	Uint32 sdl_flags = SDL_INIT_TIMER |
	                   SDL_INIT_AUDIO |
	                   SDL_INIT_VIDEO |
	                   SDL_INIT_JOYSTICK |
	                   SDL_INIT_HAPTIC |
	                   SDL_INIT_GAMECONTROLLER |
	                   SDL_INIT_EVENTS
	;
	if(SDL_Init(sdl_flags) != 0)
		SDL_Log("%s\n", SDL_GetError());
}

/* Init flags are defined in the 'macros.hpp' header */
App::App(const Uint8 flags)
{
	/* Remove the flags you don't need */
	Uint32 sdl_flags = SDL_INIT_TIMER |
	                   SDL_INIT_AUDIO |
	                   SDL_INIT_VIDEO |
	                   SDL_INIT_JOYSTICK |
	                   SDL_INIT_HAPTIC |
	                   SDL_INIT_GAMECONTROLLER |
	                   SDL_INIT_EVENTS |
	                   SDL_INIT_NOPARACHUTE
	;
	if(SDL_Init(sdl_flags) != 0)
		SDL_Log("%s\n", SDL_GetError());

	if(flags & INIT_IMAGE)
	{
		/* Remove the flags you don't need */
		Uint8 img_flags = IMG_INIT_JPG |
		                  IMG_INIT_PNG |
		                  IMG_INIT_TIF |
		                  IMG_INIT_WEBP |
		                  IMG_INIT_JXL |
		                  IMG_INIT_AVIF
		;
		if(IMG_Init(img_flags) != img_flags)
			SDL_Log("%s\n", IMG_GetError());
	}

	if((flags & INIT_TTF) && (TTF_Init() != 0))
		SDL_Log("%s\n", TTF_GetError());

	if(flags & INIT_MIXER)
	{
		/* Remove the flags you don't need */
		int mix_flags = MIX_INIT_FLAC |
		                MIX_INIT_MOD |
		                MIX_INIT_MP3 |
		                MIX_INIT_OGG |
//		                MIX_INIT_MID | // NOTE: This flag causes a memory leak.
		                MIX_INIT_OPUS |
		                MIX_INIT_WAVPACK
		;
		if(Mix_Init(mix_flags) != mix_flags)
			SDL_Log("%s\n", Mix_GetError());
	}

	if((flags & INIT_NET) && (SDLNet_Init() != 0))
		SDL_Log("%s\n", SDLNet_GetError());
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
	if(instance == nullptr)
		instance = std::shared_ptr<App> (new App(flags));
	return instance;
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
	front = Front::getInstance(windowName);
	return front != nullptr;
}

bool App::initBack()
{
	back = Back::getInstance();
	return back != nullptr;
}

/* TODO: Add a loading screen before init calls */
Sint32 App::run(const std::function<void(std::shared_ptr<Front>, std::shared_ptr<Back>)> init,
				const std::function<Sint32(std::shared_ptr<Front>, std::shared_ptr<Back>)> handle)
{
	init(front, back);

	Sint32 result = PASS;
	bool works = true;

	while(works && result == PASS)
	{
		result = back -> update(handle, front);
		works = front -> render();
		SDL_Delay(1);
	}

	front -> clearViews();
	back -> clearInputs();
	back -> clearEntities();

	return result;
}
