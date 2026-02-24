#include "front.hpp"


SDL_Renderer* View::renderer = nullptr;
std::shared_ptr<Front> Front::instance = nullptr;


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
	return View::renderer;
}

/* Used only by the Front object */
void Renderer::set(SDL_Renderer* const newRenderer)
{
	if(newRenderer != nullptr)
		View::renderer = newRenderer;
}


/* Object for source and destination SDL_Rect
 * variables
 */
Rect::Rect() {}

Rect::Rect(const SDL_Rect rect)
{
	this -> rect = rect;
}

Rect::Rect(const int x, const int y, const int w, const int h)
{
	rect = {x, y, w, h};
}

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



Static::~Static()
{
	if(texture != nullptr)
		SDL_DestroyTexture(texture);
}

/* This method is in Static and not Frame only
 * because there might be some cases where you
 * want to use it for the Canvas object
 */
bool Static::rgba(const Uint8* const r,
                  const Uint8* const g,
                  const Uint8* const b,
                  const Uint8* const a)
{
	Uint8 oldR, oldG, oldB, oldA;
	if(SDL_GetTextureColorMod(texture, &oldR, &oldG, &oldB) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	if(SDL_GetTextureAlphaMod(texture, &oldA) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	if (SDL_SetTextureColorMod(texture,
                               r == nullptr ? oldR : *r,
                               g == nullptr ? oldG : *g,
                               b == nullptr ? oldB : *b) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	if (SDL_SetTextureAlphaMod(texture,
		a == nullptr ? oldA : *a) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	return true;
}


/* Polymorphic Rect methods */
SDL_Rect Frame::getSrc()
{
	return src -> get();
}

void Frame::setSrc(SDL_Rect newSrc)
{
	src -> set(newSrc);
}

SDL_Rect Frame::getDst()
{
	return dst -> get();
}

void Frame::setDst(SDL_Rect newDst)
{
	dst -> set(newDst);
}

bool Frame::draw()
{
	SDL_Rect _src = src -> get();
	SDL_Rect _dst = dst -> get();
	if(SDL_RenderCopy(renderer, texture, &_src, &_dst) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	return true;
}



Anim::Anim()
{
	index = 0;
}

Anim::~Anim()
{
	for(SDL_Texture* frame : frames)
		SDL_DestroyTexture(frame);
}

/* NOTE: This draw method being defined in Anim
 * slightly changes how Gif works (see Gif::next)
 */
bool Anim::draw()
{
	SDL_Rect _src = src -> get();
	SDL_Rect _dst = dst -> get();
	if(SDL_RenderCopy(renderer, frames[index], &_src, &_dst) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	return true;
}

int Anim::length()
{
	return frames.size();
}

/* Polymorphic Rect methods */
SDL_Rect Anim::getSrc()
{
	return src -> get();
}

void Anim::setSrc(SDL_Rect newSrc)
{
	src -> set(newSrc);
}

SDL_Rect Anim::getDst()
{
	return dst -> get();
}

void Anim::setDst(SDL_Rect newDst)
{
	dst -> set(newDst);
}



Canvas::Canvas()
{
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, APP_W, APP_H);
	if(texture == nullptr)
		SDL_Log("%s\n", SDL_GetError());
	else
		src = std::make_unique<Rect>(0, 0, APP_W, APP_H);
}

/* Polymorphic Rect methods (source) */
SDL_Rect Canvas::getSrc()
{
	return src -> get();
}

void Canvas::setSrc(SDL_Rect newSrc)
{
	src -> set(newSrc);
}

/* Clears screen and redirects target to itself */
bool Canvas::clear()
{
	if(SDL_RenderClear(renderer) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	if(SDL_SetRenderTarget(renderer, texture) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	if(SDL_RenderClear(renderer) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	return true;
}

/* Resets the render target and prints the textures */
bool Canvas::draw()
{
	if(SDL_SetRenderTarget(renderer, nullptr) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	SDL_Rect s = src -> get();
	if(SDL_RenderCopy(renderer, texture, &s, nullptr) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	SDL_RenderPresent(renderer);
	return true;
}



Image::Image(const char* const path)
{
	SDL_Surface* surface = IMG_Load(path);
	if(surface == nullptr)
		SDL_Log("%s\n", IMG_GetError());
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if(texture == nullptr)
			SDL_Log("%s\n", SDL_GetError());
		else
		{
			int w, h;
			if(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h) != 0)
				SDL_Log("%s\n", SDL_GetError());
			else
			{
				SDL_Rect rect = {0, 0, w, h};
				src = std::make_unique<Rect>(rect);
				dst = std::make_unique<Rect>(rect);
			}
		}
	}
}



Text::Text(const char* const text, const char* const fontPath, const short size, const SDL_Color color)
{
	TTF_Font* font = TTF_OpenFont(fontPath, size);
	if(font == nullptr)
		SDL_Log("%s\n", TTF_GetError());
	else
	{
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
		TTF_CloseFont(font);
		if(surface == nullptr)
			SDL_Log("%s\n", TTF_GetError());
		else
		{
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			if(texture == nullptr)
				SDL_Log("%s\n", SDL_GetError());
			else
			{
				int w, h;
				if(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h) != 0)
					SDL_Log("%s\n", SDL_GetError());
				else
				{
					SDL_Rect rect = {0, 0, w, h};
					src = std::make_unique<Rect>(rect);
					dst = std::make_unique<Rect>(rect);
				}
			}
		}
	}
}



Gif::Gif(const char* const path) : Anim()
{
	IMG_Animation* gif = IMG_LoadAnimation(path);
	if(gif == nullptr)
		SDL_Log("%s\n", IMG_GetError());
	else
	{
		for(int i = 0; i < gif -> count; i++)
		{
			frames.push_back(SDL_CreateTextureFromSurface(renderer, gif -> frames[i]));
			delays.push_back(gif -> delays[i]);
		}

		SDL_Rect rect = {0, 0, gif -> w, gif -> h};
		src = std::make_unique<Rect>(rect);
		dst = std::make_unique<Rect>(rect);

		IMG_FreeAnimation(gif);

		start = Timestamp::now_ms();
	}
}

/* Method needed to play the animation correctly */
bool Gif::next()
{
	if(Timestamp::now_ms() > start + delays[index])
	{
		/* Using a loop here in case the program stops for
		 * a while and needs to make up for lost time
		 */
		while(start + delays[index] < Timestamp::now_ms())
		{
			start += delays[index];
			index++;
			index %= length();
		}
		return true;
	}

	return false;
}



/* Unlike Gif, Spritesheet doesn't need delays
 * between frames
 */
Spritesheet::Spritesheet(const char* const path) : Anim()
{
	IMG_Animation* sprites = IMG_LoadAnimation(path);
	if(sprites == nullptr)
		SDL_Log("%s\n", IMG_GetError());
	else
	{
		for(int i = 0; i < sprites -> count; i++)
			frames.push_back(SDL_CreateTextureFromSurface(renderer, sprites -> frames[i]));

		SDL_Rect rect = {0, 0, sprites -> w, sprites -> h};
		src = std::make_unique<Rect>(rect);
		dst = std::make_unique<Rect>(rect);

		IMG_FreeAnimation(sprites);
	}
}

/* You should use these two methods with defined
 * constants, like the ones for the keys and
 * clicks in 'macros.hpp'
 */
int Spritesheet::getIndex()
{
	return index;
}

void Spritesheet::setIndex(const int newIndex)
{
	index = newIndex % length();
}



Front::Front(const char* const name)
{
	if(name == nullptr)
		SDL_Log("window name is null\n");
	else
	{
		if((window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, APP_W, APP_H, SDL_WINDOW_SHOWN)) == nullptr)
			SDL_Log("%s\n", SDL_GetError());
		else
		{
			if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == nullptr)
				SDL_Log("%s\n", SDL_GetError());
			else
			{
				Renderer::set(renderer);
				canvas = std::make_unique<Canvas>();
			}
		}
	}
}

Front::~Front()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

/* Singleton: for one App there should only be
 * one front-end object
 */
std::shared_ptr<Front> Front::getInstance(const char* const name)
{
	if(instance == nullptr)
		instance = std::shared_ptr<Front> (new Front(name));
	return instance;
}

bool Front::setFullscreen(const bool fullscreen)
{
	if(SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0) != 0)
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	return true;
}

std::shared_ptr<Image> Front::addImage(const char* const path)
{
	if(path == nullptr)
		SDL_Log("image path is null\n");
	else
	{
		std::shared_ptr<Image> img = std::make_shared<Image>(path);
		views.push_back(img);
		images.push_back(img);
		return img;
	}

	return nullptr;
}

std::shared_ptr<Text> Front::addText(const char* const text, const char* const fontPath, const short size, const SDL_Color color)
{
	if(text == nullptr)
		SDL_Log("text string is null\n");
	else if(fontPath == nullptr)
		SDL_Log("font path is null\n");
	else if(size <= 0)
		SDL_Log("text size is too low\n");
	else
	{
		std::shared_ptr<Text> txt = std::make_shared<Text>(text, fontPath, size, color);
		views.push_back(txt);
		texts.push_back(txt);
		return txt;
	}

	return nullptr;
}

std::shared_ptr<Gif> Front::addGif(const char* const path)
{
	if(path == nullptr)
		SDL_Log("gif path is null\n");
	else
	{
		std::shared_ptr<Gif> gif = std::make_shared<Gif>(path);
		views.push_back(gif);
		gifs.push_back(gif);
		return gif;
	}

	return nullptr;
}

std::shared_ptr<Spritesheet> Front::addSpritesheet(const char* const path)
{
	if(path == nullptr)
		SDL_Log("spritesheet path is null\n");
	else
	{
		std::shared_ptr<Spritesheet> spr = std::make_shared<Spritesheet>(path);
		views.push_back(spr);
		spritesheets.push_back(spr);
		return spr;
	}

	return nullptr;
}

void Front::clearViews()
{
	views.clear();
	images.clear();
	texts.clear();
	gifs.clear();
	spritesheets.clear();
}

bool Front::render()
{
	/* Loop needed to show GIFs correctly */
	for(unsigned long i = 0; i < gifs.size(); i++)
		gifs[i] -> next();

	/* Actual render */
	if(!(canvas -> clear()))
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	for(std::shared_ptr<View> view : views)
	{
		if(!(view -> draw()) || gifs.size() < 0)
		{
			SDL_Log("%s\n", SDL_GetError());
			return false;
		}
	}

	if(!(canvas -> draw()))
	{
		SDL_Log("%s\n", SDL_GetError());
		return false;
	}

	return true;
}
