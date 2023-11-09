all: sizemain.c stringsmain.c sizemain.h stringsmain.h
	gcc -o size sizemain.c
	gcc -o strings stringsmain.c

size: sizemain.c sizemain.h
	gcc -o size sizemain.c

strings: stringsmain.c stringsmain.h
	gcc -o strings stringsmain.c

clean:
	rm -rf *.o
