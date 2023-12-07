zip:
	zip milestone3.zip *.c *.h

all: main.c sbuffer.c sbuffer.h config.h main.h
	gcc -g *.c -o main.out -lpthread

test: test.c
	gcc -g test.c sbuffer.c -o main.out -lpthread