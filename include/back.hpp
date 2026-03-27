/* Classes used for back-end management */

#pragma once

#include <common.hpp>
#include <wrap.hpp>
#include <front.hpp>

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
}
inputs_t;

/* Spritesheet option for the children classes of
 * Input, if you want a rendered button
 */
class SpritesAddon
{
protected:
	std::shared_ptr<Spritesheet> spritesheet;
public:
	SpritesAddon();
	void setSpritesheet(const std::shared_ptr<Spritesheet> newSpritesheet);
};

/* Sound effect option for the children classes
 * of Input, if you want a rendered button
 */
class SfxAddon
{
protected:
	std::shared_ptr<Chunk> chunk;
public:
	SfxAddon();
	void setSfx(const std::shared_ptr<Chunk> newChunk);
};

/* User inputs wrapper to handle the inputs
 * related variables easily
 */
class UserInputs
{
protected:
	static std::shared_ptr<inputs_t> userInputs;

public:
	static const std::shared_ptr<inputs_t> get();
	static void set(std::shared_ptr<inputs_t> newUserInputs);
};

/* Small abstract class made for the vectors in the Back class
 */
class Pressable : public Instantiable
{
public:
	Pressable() = default;
	virtual Uint32 pressed() = 0;
};

/* Protected Input class, for the mouse and
 * keyboard in its children classes
 * (Controller support might get added)
 */
template<typename... Addons>
class Input : public Pressable, public Addons...
{
protected:
	bool wasPressed;
	Input();
};

/* Class for click events, only detects if the
 * button is pressed if no Spritesheet, animates
 * the said Spritesheet otherwise
 */
template<typename... Addons>
class Click final : public Input<Addons...>
{
private:
	Uint32 click;
	bool wasHovered;

public:
	Click(const Uint32 click);
	Uint32 pressed() override;

};

/* Class for keyboard events, plays an animation
 * according to whether the corresponding key is
 * pressed or not if there is a Spritesheet,
 * only returns its state otherwise
 */
template<typename... Addons>
class Key final : public Input<Addons...>
{
private:
	Uint8 value;

public:
	Key(const Uint8 value);
	Uint32 pressed() override;
};

/* Empty Entity class for you to define, it may
 * have its own header and source file in the
 * future
 */
class Entity;

/* Object that manages all of the back-end */
class Back : public Instantiable
{
private:
	static std::shared_ptr<Back> instance;
	std::vector<std::shared_ptr<Pressable>> clicks;
	std::vector<std::shared_ptr<Pressable>> keys;
	std::vector<std::shared_ptr<Entity>> entities; // You might want to make this a vector of a child class of Entity
	Back();

public:
	static std::shared_ptr<Back> getInstance();
	void addClick(const Uint32 click);
	void addClick(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet);
	void addClick(const Uint32 click, const std::shared_ptr<Chunk> chunk);
	void addClick(const Uint32 click, const std::shared_ptr<Spritesheet> spritesheet, const std::shared_ptr<Chunk> chunk);
	void addKey(const Uint8 value);
	void addKey(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet);
	void addKey(const Uint8 value, const std::shared_ptr<Chunk> chunk);
	void addKey(const Uint8 value, const std::shared_ptr<Spritesheet> spritesheet, const std::shared_ptr<Chunk> chunk);
	void addEntity(/* ... */); // Complete this to make your own Entity adder
	bool updateEvent();
	void updateKeys();
	void updateMouse();
	const std::vector<std::shared_ptr<Pressable>>& getKeys() const;
	const std::vector<std::shared_ptr<Pressable>>& getClicks() const;
	void clearInputs();
	void clearEntities();
	Sint32 update(std::function<Sint32(const std::shared_ptr<Front>, std::shared_ptr<Back>)> handle,
                  const std::shared_ptr<Front> front);
};
