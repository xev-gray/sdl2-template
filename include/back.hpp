/* Classes used for back-end management */

#pragma once

#include "macros.hpp"
#include "front.hpp"

/* Struct to handle the mouse, keyboard and
 * closing the app
 */
typedef struct inputs
{
	SDL_Event event;
	const Uint8* keys;
	Uint32 mouseClicks;
	int mouseX;
	int mouseY;
} inputs_t;

/* Protected Input class, for the mouse and
 * keyboard in its children classes
 * (Controller support might get added)
 */
class Input
{
protected:
	std::shared_ptr<Spritesheet> spritesheet;
	bool wasPressed;
	Input();
	Input(const char* const path);
	Input(const std::shared_ptr<Spritesheet> spritesheet);
};

/* Class for click events, only detects if the
 * button is pressed if no Spritesheet, animates
 * the said Spritesheet otherwise
 */
class Click final : public Input
{
private:
	Uint32 click;
	bool wasHovered;

public:
	Click(const Uint32 click);
	Click(const Uint32 click, const char* const path);
	Click(const Uint32 click, std::shared_ptr<Spritesheet> spritesheet);
	Uint32 pressed(const inputs_t& userInputs);
};

/* Class for keyboard events, plays an animation
 * according to whether the corresponding key is
 * pressed or not if there is a Spritesheet,
 * only returns its state otherwise
 */
class Key final : public Input
{
private:
	Uint8 value;

public:
	Key(const Uint8 value);
	Key(const Uint8 value, const char* const path);
	Key(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet);
	Uint8 pressed(const Uint8* keys);
};

/* Empty Entity class for you to define, it may
 * have its own header and source file in the
 * future
 */
class Entity;

/* Object that manages all of the back-end */
class Back
{
private:
	static std::shared_ptr<Back> instance;
	std::vector<std::shared_ptr<Key>> keys;
	std::vector<std::shared_ptr<Click>> clicks;
	std::vector<std::shared_ptr<Entity>> entities; // You might want to make this a vector of a child class of Entity
	inputs_t userInputs;
	Back();

public:
	static std::shared_ptr<Back> getInstance();
	void addClick(const Uint32 click);
	void addClick(const Uint32 click, const char* const path);
	void addClick(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet);
	void addKey(const Uint8 value);
	void addKey(const Uint8 value, const char* const path);
	void addKey(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet);
	void addEntity(/* ... */); // Complete this to make your own Entity adder
	bool updateEvent();
	void updateKeys();
	void updateMouse();
	const std::vector<std::shared_ptr<Key>>& getKeys() const;
	const std::vector<std::shared_ptr<Click>>& getClicks() const;
	const inputs_t& getUserInputs() const;
	void clearInputs();
	void clearEntities();
	Sint32 update(std::function<Sint32(const std::shared_ptr<Front>, std::shared_ptr<Back>)> handle,
                  const std::shared_ptr<Front> front);
};
