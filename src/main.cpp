/* Main template */

#include <inits.hpp>
#include <handles.hpp>


int main(int argc, char** argv)
{
	/* Use the flags defined in 'common.hpp' for the
	 * App constructor (INIT_SDL is ignored)
	 */
	std::shared_ptr<App> app = App::getInstance(INIT_IMAGE | INIT_NET | INIT_MIXER | INIT_TTF);
	if(!(app -> isInstantiated())) return 1;
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

	/* Vector used to dynamically choose the
	 * right BGM (BackGround Music)
	 */
	const std::vector<const char*> bgms =
	{
		nullptr, // WARNING: Do NOT remove this null pointer
		         // or everything else will be shifted by 1
		"res/ost/invincible.mp3",
		nullptr
	};

	/* ID of the functions and BGM loaded on startup */
	Sint32 select = WELCOME;

	/* Automatic execution: the return value of each
	 * function is the ID of the next function to call
	 *
	 * NOTE: Do not forget to add your own IDs to the
	 * 'common.hpp' header, it's the same one for an
	 * init function and its corresponding handle
	 * function, so they must have the same index.
	 */
	while(select != EXIT)
		select = app -> run(inits[select], handles[select], bgms[select]);

	return 0;
}
