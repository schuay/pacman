CC=g++
SOURCES=Sounds.cpp Settings.cpp Pacman.cpp Main.cpp Log.cpp hScore.cpp Ghost.cpp Game.cpp Error.cpp BckgrObj.cpp BHeap.cpp App.cpp
OBJECTS=Sounds.o Settings.o Pacman.o Main.o Log.o hScore.o Ghost.o Game.o Error.o BckgrObj.o BHeap.o App.o
LIBS=-lSDL_ttf -lSDL_gfx -lSDL_mixer -lSDL_image
EXECUTABLE=pacman_v4

all: 
	$(CC) -c `sdl-config --cflags` $(SOURCES) 
	$(CC) `sdl-config --libs` $(LIBS)  $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f $(OBJECTS)
	
uninstall:
	rm -f $(EXECUTABLE)
