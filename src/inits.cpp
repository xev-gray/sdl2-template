/* Init functions are made to add textures and
 * inputs to the app and handle them in
 * 'handle.cpp'. Use the template at the end of
 * this file to make a new one.
 *
 * NOTE: Do not forget to also write them in the
 * header and in the 'inits' vector in the main.
 */

#include "inits.hpp"


/* WARNING
 * The pass function must NEVER be removed.
 * It is made so the program doesn't crash if
 * the function gets called accidentally.
 */
void initPass(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back) {}

void initWelcome(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back)
{
	// Front
	auto welc = front -> addText("Welcome to the demo", "res/fonts/comic-sans-ms.ttf", 90, {200, 200, 200, 255});
	welc -> setDst(
	{
		(APP_W - (welc -> getDst()).w) / 2,
		0,
		RECT_UNCHANGED,
		RECT_UNCHANGED
	});

	auto sdl = front -> addImage("res/img/sdl-logo.webp");
	sdl -> setDst(
	{
		(APP_W - (sdl -> getDst()).w) / 2,
		(APP_H - (sdl -> getDst()).h) / 2,
		RECT_UNCHANGED,
		RECT_UNCHANGED
	});

	auto next = front -> addText("Right arrow: go to input test page", "res/fonts/arial.ttf", 40, {255, 255, 255, 255});
	next -> setDst(
	{
		0,
		APP_H - (next -> getDst()).h,
		RECT_UNCHANGED,
		RECT_UNCHANGED
	});

	auto readme = front -> addText("Don't forget to check the README.md!", "res/fonts/comic-sans-ms.ttf", 30, {255, 64, 50, 255});
	readme -> setDst(
	{
		(APP_W - (readme -> getDst()).w) / 2,
		APP_H - (readme -> getDst()).h - (next -> getDst()).h,
		RECT_UNCHANGED,
		RECT_UNCHANGED
	});

	// Back
	back -> addKey(SDL_SCANCODE_RIGHT);
}

void initInputTest(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back)
{
	// Front
	auto click = front -> addSpritesheet("res/gif/click-button.gif");
	click -> setDst (
	{
		(APP_W - click -> getDst().w) / 2,
		(APP_H - click -> getDst().h) / 4,
		RECT_UNCHANGED,
		RECT_UNCHANGED
	});

	auto key = front -> addSpritesheet("res/gif/key-button.gif");
	key -> setDst (
	{
		(APP_W - key -> getDst().w) / 2,
		(APP_H - key -> getDst().h) / 4 * 3,
		RECT_UNCHANGED,
		RECT_UNCHANGED
	});

	auto prev = front -> addText("Left arrow: go back to welcome page", "res/fonts/arial.ttf", 40, {255, 255, 255, 255});
	prev -> setDst(
	{
		0,
		APP_H - (prev -> getDst()).h,
		RECT_UNCHANGED,
		RECT_UNCHANGED
	});

	// Back
	back -> addClick(SDL_BUTTON_LEFT, click);
	back -> addKey(SDL_SCANCODE_SPACE, key);
	back -> addKey(SDL_SCANCODE_LEFT);
}

/* Handle function template

void initTemplate(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back)
{
	// Front
	// Add rendered objects here

	// Back
	// Add input-based objects here
}

*/
