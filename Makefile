SOURCES=$(filter-out src/main.c,$(wildcard src/*.c))
HEADERS=$(wildcard include/*.h)

MAIN=src/main.c

TEST_CASES=$(filter-out test/main.c,$(wildcard test/*.c))
TEST_MAIN=test/main.c

TEST_HEADERS=$(wildcard test/*.h)
TEST_EXES=$(patsubst test/%.c, test/bin/%, $(TEST_CASES))

CFLAGS=-g -DSTOPWATCH_ENABLED

all: sdl_game 

sdl_game: $(SOURCES) $(HEADERS) $(MAIN) 
	gcc $(CFLAGS) -I./include -o sdl_game  $(SOURCES) $(MAIN) -lm -lSDL -lSDL_image -lGL

test/bin: 
	mkdir -p test/bin

test/bin/% : test/%.c  $(C_DEFS) $(SOURCES) $(HEADERS) $(TEST_MAIN) $(TEST_HEADERS) test/bin
	gcc -g -I./include -I./test -o $@  $(C_DEFS) $(SOURCES) $(TEST_MAIN) $<  -lm -lSDL -lSDL_image -lGL

build_tests: $(TEST_EXES)

test: build_tests
	@((for t in test/bin/*; do                      \
	    $${t} 2>&1;                                 \
	   done) | awk 'BEGIN {c=0; f=0}                \
                        /\.\.\.ok$$/ {c=c+1}            \
                        /\.\.\.FAILED$$/ {c=c+1; f=f+1} \
                        { print $$0; }                  \
                        END { if (0 < f){ print f" of "c" failed"; exit 1} }')

clean: 
	rm -f sdl_game
	rm -f test/bin/*

