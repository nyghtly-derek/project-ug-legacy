#OBJS specifies which files to compile as part of the project
SRC = $(wildcard src/*.cpp)
HEADER = $(wildcard include/*.h)

#CC specifies which compiler we're using, we default to clang++
CC ?= clang++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -Iinclude `sdl2-config --cflags --libs`

LINK_FLAGS = -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = play-gdg

#This is the target that compiles our executable
all : $(SRC) $(HEADER)
	$(CC) $(SRC) $(COMPILER_FLAGS) -o $(OBJ_NAME) $(LINK_FLAGS)

.PHONY: clean
clean:
	rm -R play-gdg
