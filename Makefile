SOURCES=$(filter-out src/main.c,$(wildcard src/*.c))
HEADERS=$(wildcard include/*.h)

MAIN=src/main.c

OBJ_FILES=$(patsubst src/%.c, obj/%.o, $(SOURCES))

TEST_CASES=$(filter-out test/main.c,$(wildcard test/*.c))
TEST_MAIN=test/main.c

TEST_HEADERS=$(wildcard test/*.h)
TEST_EXES=$(patsubst test/%.c, test/bin/%, $(TEST_CASES))

CFLAGS=-I./include -g -DSTOPWATCH_ENABLED

LDLIBS=-lm -lSDL -lSDL_image -lGL

all: sdl_game 

sdl_game : $(OBJ_FILES) obj/main.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

obj/background.o : src/background.c obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -ansi -pedantic-errors -Wall -Werror -c -o $@ $<

obj/collision.o : src/collision.c obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -ansi -pedantic-errors -Wall -Werror -c -o $@ $<

obj/control.o : src/control.c obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -ansi -pedantic-errors -Wall -Werror -c -o $@ $<

obj/utils.o : src/utils.c obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -ansi -pedantic-errors -c -o $@ $<

obj/events.o : src/events.c obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -ansi -pedantic-errors -c -o $@ $<


obj/%.o : src/%.c obj
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

obj :
	mkdir -p obj

test/bin: 
	mkdir -p test/bin

test/obj:
	mkdir -p test/obj

.PRECIOUS: test/obj/%.o
test/obj/%.o : test/%.c test/obj
	$(CC) -g -I./include -I./test -c -o $@ $<

test/bin/% : test/obj/%.o test/obj/main.o $(OBJ_FILES) $(HEADERS) $(TEST_HEADERS) test/bin
	$(CC) $(LDFLAGS) $< test/obj/main.o $(OBJ_FILES) $(LOADLIBES) $(LDLIBS) -o $@

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
	rm -f obj/*
	rm -f test/bin/*
	rm -f test/obj/*

