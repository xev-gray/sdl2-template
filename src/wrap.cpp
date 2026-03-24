#include <wrap.hpp>


SDL_Renderer* Renderer::renderer = nullptr;
Mix_Music* Music::music = nullptr;
std::shared_ptr<Music> Music::instance = nullptr;

bool Instantiable::isInstantiated()
{
	return instantiated;
}



/* Returns the Unix timestamp */
unsigned long Timestamp::now()
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

/* Returns the Unix timestamp in milliseconds */
unsigned long Timestamp::now_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

/* Returns a decimal version of the Unix timestamp */
double Timestamp::now_double()
{
	return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}



/* In case back-end objects need the renderer */
SDL_Renderer* const Renderer::get()
{
	return renderer;
}

/* Used only by the Front object */
void Renderer::set(SDL_Window* const window, const int index, const Uint32 flags)
{
	renderer = SDL_CreateRenderer(window, index, flags);
	if(renderer == nullptr)
		SDL_Log("%s\n", SDL_GetError());
}

/* Used only by the Front object */
void Renderer::set(SDL_Renderer* const newRenderer)
{
	if(newRenderer == nullptr)
		SDL_Log("Cannot change renderer: new renderer is null\n");
	else
		renderer = newRenderer;
}

/* This method is already in Front's destructor */
void Renderer::destroy()
{
	SDL_DestroyRenderer(renderer);
}



/* Object for source and destination SDL_Rect
 * variables
 */
Rect::Rect() {}

Rect::Rect(const SDL_Rect rect) : rect(rect) {}

Rect::Rect(const int x, const int y, const Uint32 w, const Uint32 h)
: rect({x, y, static_cast<int>(w), static_cast<int>(h)}) {}

SDL_Rect Rect::get()
{
	return rect;
}

void Rect::set(SDL_Rect newRect)
{
	if(newRect.w <= 0)
		newRect.w = rect.w;

	if(newRect.h <= 0)
		newRect.h = rect.h;

	rect = newRect;
}



/* Simple audio opening, commonly used */
bool Audio::open()
{
	if(!(init_flags & INIT_MIXER))
	{
		SDL_Log("Cannot open audio device: SDL2_mixer hasn't been initialized\n");
		return false;
	}
	else
	{
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
		{
			SDL_Log("%s\n", Mix_GetError());
			return false;
		}
		return true;
	}
}

/* Specific audio opening for more control */
bool Audio::open(const char* const device, int allowChanges)
{
	if(device == nullptr)
	{
		SDL_Log("Cannot open audio device: Device name is null\n");
		return false;
	}
	else if(!(init_flags & INIT_MIXER))
	{
		SDL_Log("Cannot open audio device '%s': SDL2_mixer is not initialized\n", device);
		return false;
	}
	else if(allowChanges == 0)
	{
		SDL_Log("Cannot open audio device '%s': Invalid allowed changes\n", device);
		return false;
	}
	else
	{
		if(Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, 2, 2048, device, allowChanges) != 0)
		{
			SDL_Log("%s\n", Mix_GetError());
			return false;
		}
		return true;
	}
}

/* Called after the loop in App::run */
void Audio::close()
{
	Mix_CloseAudio();
}



/* Loops getter and setter */
int Mix::getLoops()
{
	return loops;
}

void Mix::setLoops(int newLoops)
{
	loops = newLoops;
}


/* Load music file using one of the flags in
 * App::App
 */
Music::Music(const char* const path)
{
	if(path == nullptr)
	{
		SDL_Log("Cannot open music file: Music path is null\n");
		instantiated = false;
	}
	else if(!(init_flags & INIT_MIXER))
	{
		SDL_Log("Cannot open music file '%s': SDL2_mixer is not initialized\n", path);
		instantiated = false;
	}
	else
	{
		music = Mix_LoadMUS(path);
		if(music == nullptr)
		{
			SDL_Log("%s\n", Mix_GetError());
			instantiated = false;
		}
		else
			instantiated = true;
	}
}

/* Called automatically thanks to the shared_ptr */
Music::~Music()
{
	if(music != nullptr)
		Mix_FreeMusic(music);
}

/* It's impossible to have multiple songs running
 * so Music is singleton
 */
std::shared_ptr<Music> Music::getInstance(const char* const path)
{
	if(path == nullptr)
	{
		SDL_Log("Cannot open music file: Music path is null\n");
		return nullptr;
	}
	else if(!(init_flags & INIT_MIXER))
	{
		SDL_Log("Cannot open music file '%s': SDL2_mixer is not initialized\n", path);
		return nullptr;
	}
	else
	{
		if(instance == nullptr)
			instance = std::shared_ptr<Music>(new Music(path));
		return instance;
	}
}

/* Volume getter and setter */
int Music::getVolume()
{
	return Mix_VolumeMusic(-1);
}

void Music::setVolume(int newVolume)
{
	Mix_VolumeMusic(newVolume);
}

/* Music only plays when this is called */
bool Music::play()
{
	return Mix_PlayMusic(music, loops) == 0;
}

/* Pause music without completely stopping it */
void Music::pause()
{
	Mix_PauseMusic();
}

/* Continue to play, only after pausing */
void Music::resume()
{
	Mix_ResumeMusic();
}

/* Stop playing music, cannot be resumed */
void Music::stop()
{
	Mix_HaltMusic();
}

/* Returns true if music is playing */
bool Music::isPlaying()
{
	return Mix_PlayingMusic();
}

/* Returns true if music is paused */
bool Music::isPaused()
{
	return Mix_PausedMusic();
}

/* Restart the current song */
void Music::rewind()
{
	Mix_RewindMusic();
}



/* Must specify in what channel the SFX must be
 * played
 */
Sfx::Sfx(const char* const path, int channel)
{
	if(path == nullptr)
	{
		SDL_Log("Cannot open SFX file: SFX path is null\n");
		chunk = nullptr;
		instantiated = false;
	}
	else
	{
		chunk = Mix_LoadWAV(path);
		if(chunk == nullptr)
		{
			SDL_Log("%s\n", Mix_GetError());
			instantiated = false;
		}
		else
		{
			this -> channel = channel;
			instantiated = true;
		}
	}
}

Sfx::~Sfx()
{
	if(chunk != nullptr)
		Mix_FreeChunk(chunk);
}

/* Volume getter and setter */
int Sfx::getVolume()
{
	return Mix_VolumeChunk(chunk, -1);
}

void Sfx::setVolume(int newVolume)
{
	Mix_VolumeChunk(chunk, newVolume);
}

/* Channel getter and setter */
int Sfx::getChannel()
{
	return channel;
}

void Sfx::setChannel(int newChannel)
{
	channel = newChannel;
}

/* SFX only plays when this is called */
bool Sfx::play()
{
	return Mix_PlayChannel(channel, chunk, loops) == 0;
}

/* Pause SFX without completely stopping it */
void Sfx::pause()
{
	Mix_Pause(channel);
}

/* Continue to play, only after pausing */
void Sfx::resume()
{
	Mix_Resume(channel);
}

/* Stop playing SFX, cannot be resumed */
void Sfx::stop()
{
	Mix_HaltChannel(channel);
}

/* Returns true if SFX is playing */
bool Sfx::isPlaying()
{
	return Mix_Playing(channel);
}

/* Returns true if SFX is paused */
bool Sfx::isPaused()
{
	return Mix_Paused(channel);
}
