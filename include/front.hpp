/* Classes used for front-end management */

#pragma once

#include <common.hpp>
#include <wrap.hpp>

/* Abstract View object for the draw() method */
class View : public Instantiable
{
public:
	View() = default;
	virtual bool draw() = 0;
};

/* Abstract Static object for other children
 * objects containing only one texture
 */
class Static : public View
{
protected:
	SDL_Texture* texture;

public:
	Static() = default;
	virtual ~Static();
	bool rgba(const Uint8* const r,
              const Uint8* const g,
              const Uint8* const b,
              const Uint8* const a);
};

/* Abstract Frame object for everything about
 * rendering a texture, only the constructor
 * differs for what we would like to do
 */
class Frame : public Static
{
protected:
	std::unique_ptr<Rect> src;
	std::unique_ptr<Rect> dst;

public:
	Frame() = default;
	SDL_Rect getSrc();
	void setSrc(SDL_Rect newSrc);
	SDL_Rect getDst();
	void setDst(SDL_Rect newDst);
	bool draw();
};

/* Abstract Anim object for other children
 * objects containing more than one texture
 */
class Anim : public View
{
protected:
	std::unique_ptr<Rect> src;
	std::unique_ptr<Rect> dst;
	std::vector<SDL_Texture*> frames;
	int index;
	Anim();

public:
	virtual ~Anim();
	bool draw();
	int length();
	SDL_Rect getSrc();
	void setSrc(SDL_Rect newSrc);
	SDL_Rect getDst();
	void setDst(SDL_Rect newDst);
};

/* Canvas object used as a target texture to
 * resize the window more easily
 */
class Canvas final : public Static
{
private:
	std::unique_ptr<Rect> src;

public:
	Canvas();
	SDL_Rect getSrc();
	void setSrc(SDL_Rect newSrc);
	bool clear();
	bool draw();
};

/* Image object, only has a constructor to load
 * one image at a time
 */
class Image final : public Frame
{
public:
	Image(const char* const path);
};

/* Text object, only has a constructor to load
 * one character string at a time
 */
class Text final : public Frame
{
public:
	Text(const char* const text, const char* const fontPath, const short size, const SDL_Color color);
};

/* Gif object, automatically loads and plays a
 * GIF file
 */
class Gif final : public Anim
{
private:
	std::vector<unsigned long> delays;
	unsigned long start;

public:
	Gif(const char* const path);
	bool next();
};

/* Spritesheet object, meant to be used for
 * back-end inputs and entities
 *
 * NOTE: The constructor loads a GIF file which
 * should have every needed texture as its frames.
 */
class Spritesheet final : public Anim
{
public:
	Spritesheet(const char* const path);
	int getIndex();
	void setIndex(const int newIndex);
};

/* Object that manages all of the front-end
 *
 * NOTE: The 'views' vector is meant to store
 * every single view, ranked from the one with
 * the deepest layer to the one with the most
 * surface layer.
 * The 'gifs' vector has to be treated separately
 * from other objects because of the Gif::next
 * method used in Front::render.
 * The other vectors might get removed in a
 * future update, for now they are there for
 * convenience.
 */
class Front : public Instantiable
{
private:
	static std::shared_ptr<Front> instance;
	SDL_Window* window;
	std::unique_ptr<Canvas> canvas;
	std::vector<std::shared_ptr<View>> views;
	std::vector<std::shared_ptr<Image>> images;
	std::vector<std::shared_ptr<Text>> texts;
	std::vector<std::shared_ptr<Gif>> gifs;
	std::vector<std::shared_ptr<Spritesheet>> spritesheets;
	Front(const char* const name);

public:
	~Front();
	static std::shared_ptr<Front> getInstance(const char* const name);
	bool setFullscreen(const bool fullscreen);
	std::shared_ptr<Image> addImage(const char* const path);
	std::shared_ptr<Text> addText(const char* const text, const char* const fontPath, const short size, const SDL_Color color);
	std::shared_ptr<Gif> addGif(const char* const path);
	std::shared_ptr<Spritesheet> addSpritesheet(const char* const path);
	void clearViews();
	bool render();
};
