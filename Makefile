SOURCES=src/main.c src/video.c src/events.c src/joystick.c src/timing.c

sdl_game: $(SOURCES)
	gcc -g -I./include -o sdl_game $(SOURCES) -lSDL -lSDL_image -lGL

clean: 
	rm -f sdl_game