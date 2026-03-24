/* Handle functions are made to manage keys and
 * mouse buttons you initialized in the
 * 'init.cpp' file. Use the template at the end
 * of this file to make a new one.
 *
 * Since the BGM resets for every App::run() call
 * you will need to call some handles from other
 * handles and call every init you need for a
 * single BGM.
 *
 * NOTE: Do not forget to also write them in the
 * header and in the 'handles' vector in the main
 * (except for handles called from other handles,
 * those should be static).
 */

#include <handles.hpp>


/* WARNING
 * The pass function must NEVER be removed.
 * It is made so the program doesn't crash if
 * the function gets called accidentally.
 */
Sint32 handlePass(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back) { return EXIT; }

Sint32 handleWelcome(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back)
{
	// Pointer handling
	if(front == nullptr)
	{
		SDL_Log("Cannot call handle function: Front-end is null\n");
		return EXIT;
	}
	else if(back == nullptr)
	{
		SDL_Log("Cannot call handle function: Back-end is null\n");
		return EXIT;
	}

	else
	{
		const inputs_t userInputs = back -> getUserInputs();
		for(std::shared_ptr<Click> click : back -> getClicks())
		{
			switch(click -> pressed(userInputs))
			{
				// ...
			}
		}

		for(std::shared_ptr<Key> key : back -> getKeys())
		{
			switch(key -> pressed(userInputs.keys))
			{
				case SDL_SCANCODE_RIGHT:
					return INPUTTEST;
					break;
			}
		}
		return PASS;
	}
}

Sint32 handleInputTest(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back)
{
	// Pointer handling
	if(front == nullptr)
	{
		SDL_Log("Cannot call handle function: Front-end is null\n");
		return EXIT;
	}
	else if(back == nullptr)
	{
		SDL_Log("Cannot call handle function: Back-end is null\n");
		return EXIT;
	}

	else
	{
		const inputs_t userInputs = back -> getUserInputs();
		for(std::shared_ptr<Click> click : back -> getClicks())
		{
			switch(click -> pressed(userInputs))
			{
				// ...
			}
		}

		for(std::shared_ptr<Key> key : back -> getKeys())
		{
			switch(key -> pressed(userInputs.keys))
			{
				case SDL_SCANCODE_LEFT:
					return WELCOME;
					break;
			}
		}
		return PASS;
	}
}

/* Handle function template

Sint32 handleTemplate(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back)
{
	// Pointer handling
	if(front == nullptr)
	{
		SDL_Log("Cannot call handle function: Front-end is null\n");
		return EXIT;
	}
	else if(back == nullptr)
	{
		SDL_Log("Cannot call handle function: Back-end is null\n");
		return EXIT;
	}

	else
	{
		const inputs_t userInputs = back -> getUserInputs();
		for(std::shared_ptr<Click> click : back -> getClicks())
		{
			switch(click -> pressed(userInputs))
			{
				// Handle click events here
			}
		}

		for(std::shared_ptr<Key> key : back -> getKeys())
		{
			switch(key -> pressed(userInputs.keys))
			{
				// Handle keys here
			}
		}
		return PASS;
	}
}

*/
