/* Main template */

#include "inits.hpp"
#include "handles.hpp"


int main(int argc, char** argv)
{
	/* Use the flags defined in 'macros.hpp' for the App constructor */
	std::shared_ptr<App> app = App::getInstance(INIT_IMAGE | INIT_MIXER | INIT_TTF | INIT_NET);
	if(!(app -> initFront("Base"))) return 1;
	if(!(app -> initBack())) return 1;

	/* Vector used to dynamically call the right init
	 * function
	 */
	const std::vector<std::function<void(std::shared_ptr<Front>, std::shared_ptr<Back>)>> inits =
	{
		initPass, // WARNING: Do NOT remove the pass function,
		          // it prevents the program from crashing
		initWelcome,
		initInputTest
	};

	/* Vector used to dynamically call the right
	 * handle function
	 */
	const std::vector<std::function<Sint32(std::shared_ptr<Front>, std::shared_ptr<Back>)>> handles =
	{
		handlePass, // WARNING: Do NOT remove the pass function,
		            // it prevents the program from crashing
		handleWelcome,
		handleInputTest
	};

	/* ID of the function called on startup */
	Sint32 func = WELCOME;

	/* Automatic execution: the return value of each
	 * function is the ID of the next function to call
	 *
	 * NOTE: Do not forget to add your own IDs to the
	 * 'macros.hpp' header, it's the same one for an
	 * init function and its corresponding handle
	 * function, so they must have the same index.
	 */
	while(func != EXIT)
		func = app -> run(inits[func], handles[func]);

	return 0;
}
