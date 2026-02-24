/* App class - the main object of the program */

#pragma once

#include "macros.hpp"
#include "front.hpp"
#include "back.hpp"


class App
{
private:
	static std::shared_ptr<App> instance;
	std::shared_ptr<Front> front;
	std::shared_ptr<Back> back;
	App();
	App(const Uint8 flags);

public:
	static std::shared_ptr<App> getInstance();
	static std::shared_ptr<App> getInstance(const Uint8 flags);
	~App();
	bool initFront(const char* const windowName);
	bool initBack();
	Sint32 run(const std::function<void(std::shared_ptr<Front>, std::shared_ptr<Back>)>,
               const std::function<Sint32(std::shared_ptr<Front>, std::shared_ptr<Back>)>);
};
