/* Handle functions header file for the main */
#pragma once

#include "macros.hpp"
#include "app.hpp"


/* WARNING: Do NOT remove the pass function. */
Sint32 handlePass(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back);

/* Add your own handle functions below */
Sint32 handleWelcome(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back);
Sint32 handleInputTest(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back);
