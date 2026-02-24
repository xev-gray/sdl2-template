/* Macro header used by all files */

#pragma once

/* SDL2 libraries */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

/* Other C++ libraries */
#include <chrono>
#include <memory>
#include <vector>
#include <functional>

/* Value for unchanging rect width/height */
#define RECT_UNCHANGED 0

/* Window size, respectively width and height */
#define APP_W 960
#define APP_H 540

/* Flags used for the App constructor */
#define INIT_IMAGE  0b0001
#define INIT_TTF    0b0010
#define INIT_MIXER  0b0100
#define INIT_NET    0b1000

/* States of a Click object */
#define CLICK_DEFAULT 0
#define CLICK_HOVERED 1
#define CLICK_PRESSED 2

/* States of a Key object */
#define KEY_DEFAULT 0
#define KEY_PRESSED 1

/* Default IDs for init and handle functions
 * (Do NOT remove)
 */
#define EXIT -1
#define PASS 0

/* Add your own function IDs below */
#define WELCOME 1
#define INPUTTEST 2
