#OBJS specifies which files to compile as part of the project
OBJS = main.c init.c textures.c player.c enemy.c bullet.c boxes.c ranking.c

#CC specifies which compiler we're using
CC = gcc

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\SDL\SDL2-2.0.7\i686-w64-mingw32\include\SDL2 -IC:\SDL\SDL2_image-2.0.2\i686-w64-mingw32\include\SDL2 -IC:\SDL\SDL2_ttf-2.0.14\i686-w64-mingw32\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\SDL\SDL2-2.0.7\i686-w64-mingw32\lib -LC:\SDL\SDL2_image-2.0.2\i686-w64-mingw32\lib -LC:\SDL\SDL2_ttf-2.0.14\i686-w64-mingw32\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
#  gets rid of the console window
COMPILER_FLAGS = -xc -std=c99 -g -Wall -lm -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = "Space Invaders"

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)