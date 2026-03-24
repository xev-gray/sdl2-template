/* Classes used for SDL2 types wrapping */

#pragma once

#include <common.hpp>

class Instantiable
{
protected:
	bool instantiated;
public:
	Instantiable() = default;
	bool isInstantiated();
};

/* Timestamp class for an easier timestamp
 * management (useful for the Gif class)
 */
class Timestamp
{
public:
	Timestamp() = delete;
	static unsigned long now();
	static unsigned long now_ms();
	static double now_double();
};

/* Renderer object used to handle the static
 * SDL_Renderer of the View object
 */
class Renderer
{
private:
	static SDL_Renderer* renderer;

public:
	Renderer() = delete;
	static SDL_Renderer* const get();
	static void set(SDL_Window* const window, const int index, const Uint32 flags);
	static void set(SDL_Renderer* const newRenderer);
	static void destroy();
};

/* SDL_Rect object used to handle the selected
 * size inside each texture and the size of the
 * rendered texture
 */
class Rect
{
protected:
	SDL_Rect rect;

public:
	Rect();
	Rect(const SDL_Rect rect);
	Rect(const int x, const int y, const Uint32 w, const Uint32 h);
	SDL_Rect get();
	void set(SDL_Rect newRect);
};

/* Small Audio class to handle SDL_OpenAudio and
 * SDL_CloseAudio more easily
 */
class Audio
{
public:
	Audio() = delete;
	static bool open();
	static bool open(const char* const path, int allowChanges);
	static void close();
};

/* Abstract class for audio files reading */
class Mix : public Instantiable
{
protected:
	int loops;
	Mix() = default;
public:
	int getLoops();
	void setLoops(int newLoops);
	virtual int getVolume() = 0;
	virtual void setVolume(int newVolume) = 0;
	virtual bool play() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void stop() = 0;
	virtual bool isPlaying() = 0;
	virtual bool isPaused() = 0;
};

/* Singleton class for the background music */
class Music final : public Mix
{
private:
	static Mix_Music* music;
	static std::shared_ptr<Music> instance;
	Music(const char* const path);
public:
	~Music();
	static std::shared_ptr<Music> getInstance(const char* const path);
	int getVolume() override;
	void setVolume(int newVolume) override;
	bool play() override;
	void pause() override;
	void resume() override;
	void stop() override;
	bool isPlaying() override;
	bool isPaused() override;
	void rewind();
};

/* Class for sound effects or music that is not
 * the background music
 */
class Sfx final : public Mix
{
private:
	Mix_Chunk* chunk;
	int channel;
public:
	Sfx(const char* const path, int channel);
	~Sfx();
	int getVolume() override;
	void setVolume(int newVolume) override;
	int getChannel();
	void setChannel(int newChannel);
	bool play() override;
	void pause() override;
	void resume() override;
	void stop() override;
	bool isPlaying() override;
	bool isPaused() override;
};
