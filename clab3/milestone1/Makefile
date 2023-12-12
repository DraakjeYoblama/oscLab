
ourtest : dplist.c ourtest.c
	mkdir -p build
	gcc dplist.c ourtest.c -o build/dplist_test -g

randomtest : dplist.c randomtest.c
	mkdir -p build
	gcc dplist.c randomtest.c -o build/dplist_test -g

all : dplist.c dplist_test.c
	mkdir -p build
	gcc dplist.c dplist_test.c -o build/dplist_test -g

clean:
	rm -r build

zip:
	zip milestone1.zip *.c *.h