SOURCES=main.c video.c events.c joystick.c timing.c

sdl_game: $(SOURCES)
	gcc -g -I. -o sdl_game $(SOURCES) -lSDL -lSDL_image -lGL

clean: 
	rm -f sdl_game