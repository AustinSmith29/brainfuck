bf:
	gcc -g -c src/bf.c src/stack.c
	ar cr build/libbf.a build/bf.o build/stack.o
	mv *.o build

all:
	gcc -g -o bin/bf src/bfi.c -L build/ -lbf
