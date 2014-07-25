SOURCES=main.c video.c events.c

sdl_game: $(SOURCES)
	gcc -g -I. -o sdl_game $(SOURCES) -lSDL -lGL

clean: 
	rm -f sdl_game