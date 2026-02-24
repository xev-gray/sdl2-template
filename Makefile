# Cross-platform Makefile template

# WARNING
# Make sure you have MinGW-w64 installed for
# both Windows compiling and cross-compiling.

NAME = base
UNAME = $(shell uname)

ifeq ($(OS), Windows_NT)   # Windows
	CXX = g++
	LIB = -Llib
	EXT = -lmingw32
	EXE = $(addsuffix .exe, $(NAME))
	TAR = $(addsuffix -win.tar, $(NAME))
	DLL = *.dll
endif

# To compile from Unix-like to Windows, replace
# all values in the following variables with the
# ones in comments.

ifeq ($(UNAME), Darwin)    # MacOS
	CXX = clang++ #x86_64-w64-mingw32-g++
	LIB = #-Llib
	EXT = #-lmingw32
	EXE = $(NAME) #$(addsuffix .exe, $(NAME))
	TAR = $(addsuffix -mac.tar, $(NAME)) #$(addsuffix -win.tar, $(NAME))
	DLL = #*.dll
endif

ifeq ($(UNAME), Linux)     # Linux
	CXX = g++ #x86_64-w64-mingw32-g++
	LIB = #-Llib
	EXT = #-lmingw32
	EXE = $(NAME) #$(addsuffix .exe, $(NAME))
	TAR = $(addsuffix -gnu.tar, $(NAME)) #$(addsuffix -win.tar, $(NAME))
	DLL = #*.dll
endif

SRC = src
BIN = bin
INC = -Iinclude
CFLAGS = -Wall
EXT += -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net
OBJ = $(addsuffix .o, $(addprefix $(BIN)/, main front back app inits handles))

# The 'zip' option is made to automatically create
# a shippable archive of your app, but it works
# only if you have tar and gzip installed on your
# device.

zip: small
	tar -cf $(TAR) res $(EXE) $(DLL)
	gzip -f $(TAR)

small: clean all mrproper

all: $(OBJ)
	$(CXX) $(INC) $(LIB) $^ -o $(EXE) $(EXT) $(CFLAGS)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(INC) $(LIB) -c $< -o $@ $(EXT) $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXE) $(addsuffix .gz, $(TAR))

mrproper:
	rm -f $(OBJ)
