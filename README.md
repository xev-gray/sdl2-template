# About

This is a free-to-use application template for games and softwares. It is written in C++ using SDL2 for a fast and powerful application. The goal was to simplify the creation of apps by using two types of files : the `inits.[ch]pp` and the `handles.[ch]pp`.

# Usage

The `inits.cpp` source file is for, well, initializing the UI by adding images and inputs respectively to the `Front`-end and `Back`-end objects, whereas the `inits.hpp` header file is for declaring the initializing functions so that they can be called in the main.
The `handles.cpp` source file is for managing the objects you added in the `inits.cpp` by handling the values of each input in a `switch` statement, so depending of its value you can choose how each input behaves. The `handles.hpp` header file serves a purpose similar to `inits.hpp`.

# Installation

If you have Git installed :
Clone this repository by typing `git clone https://github.com/xev-gray/sdl2-template.git` in your terminal.

If you don't :
Navigate to the top of this page, click the green `<> Code` button, then click "Download ZIP", and then extract the files from the archive once it's downloaded.

# Configuration

### Windows
Everything you need should be in the `windows.tar.gz` archive. Just extract its content (you may have to do it twice) and you're good to go.
However, you need Git's Bash interface to be able to build the project. Install it by clicking [here](https://gitforwindows.org/).

### Linux
You need to install GCC, Make and all of the SDL2 libraries in order to run the project. Use the appropriate command for your distro.
- Debian or Debian-based : `sudo apt install gcc make libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev`
- Fedora or Fedora-based : `sudo dnf install gcc make SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel SDL2_net-devel`
- Arch or Arch-based : `sudo pacman -S gcc make sdl2 sdl2_image sdl2_mixer sdl2_net sdl2_ttf`
If your distro isn't listed above, you can always install SDL2 by following [these instructions](https://wiki.libsdl.org/SDL2/Installation).

### MacOS
Paste this into your terminal : 
```
xcode-select --install
brew install sdl2 sdl2_image sdl2_mixer sdl2_net sdl2_ttf
```

# TODO

- Add a loading screen for long transitions
- Implement classes for the base SDL2 functions
- Implement classes for SDL2_net
- Use CMake **alongside** Makefile
- BONUS: Add Vulkan support
