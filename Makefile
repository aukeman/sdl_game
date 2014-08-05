SOURCES=$(wildcard src/*.c)
HEADERS=$(wildcard include/*.h)

sdl_game: $(SOURCES) $(HEADERS)
	gcc -g -I./include -o sdl_game $(SOURCES) -lSDL -lSDL_image -lGL

clean: 
	rm -f sdl_game