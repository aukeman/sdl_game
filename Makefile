SOURCES=$(filter-out src/main.c,$(wildcard src/*.c))
HEADERS=$(wildcard include/*.h)

MAIN=src/main.c

TEST_CASES=$(filter-out test/main.c,$(wildcard test/*.c))
TEST_MAIN=test/main.c

TEST_EXES=$(patsubst test/%.c, test/bin/%, $(TEST_CASES))

all: sdl_game test

sdl_game: $(SOURCES) $(HEADERS) $(MAIN) 
	gcc -g -I./include -o sdl_game $(SOURCES) $(MAIN) -lSDL -lSDL_image -lGL

test/bin: 
	mkdir -p test/bin

test/bin/% : test/%.c $(SOURCES) $(HEADERS) $(TEST_MAIN) test/bin
	gcc -g -I./include -I./test -o $@ $(SOURCES) $(TEST_MAIN) $<  -lSDL -lSDL_image -lGL

build_tests: $(TEST_EXES)

test: build_tests
	@(result=true;                         \
	  for t in test/bin/*; do              \
	    if ! $${t}; then result=false; fi  \
	  done;                                \
	  $${result})

clean: 
	rm -f sdl_game
	rm -f test/tests

