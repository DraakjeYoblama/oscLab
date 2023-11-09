
all : dplist.c dplist.h program.c
	mkdir -p build
	gcc dplist.c program.c -o build/dplistprogram -std=c11 -Wall -g

memtest: all
	valgrind --leak-check=yes ./build/dplistprogram

clean:
	rm -fr *.o build/*
	rm -r build

