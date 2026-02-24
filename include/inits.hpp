/* Init functions header file for the main */
#pragma once

#include "macros.hpp"
#include "app.hpp"


/* WARNING: Do NOT remove the pass function. */
void initPass(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back);

/* Add your own init functions below */
void initWelcome(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back);
void initInputTest(const std::shared_ptr<Front> front, const std::shared_ptr<Back> back);
