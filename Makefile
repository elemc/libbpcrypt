NAME=libbpcrypt

all: shared

shared: libbpcrypt.o bptree.o
	gcc -shared -o $(NAME).so libbpcrypt.o bptree.o -lssl -g

static:
	gcc -c libbpcrypt.c
	ar rs $(NAME).a libbpcrypt.o

clean:
	rm -rf libbpcrypt.so libbpcrypt.a libbpcrypt.o test libbpcrypt.dylib

libbpcrypt.o:
	gcc -c -fPIC libbpcrypt.c -g

bptree.o:
	gcc -c -fPIC bptree.c -g

test: shared
	gcc -I. -L. -o test test.c -lbpcrypt -lcrypto -g
	gcc -I. -L. -o test-tree test.c -lbpcrypt -g

macosx:
	clang -dynamiclib -o libbpcrypt.dylib -lssl -lcrypto libbpcrypt.c bptree.c -Wno-deprecated-declarations
	clang -I. -L. -o test libbpcrypt.dylib test.c -lcrypto -Wno-deprecated-declarations
	clang -I. -L. -o test-tree libbpcrypt.dylib test-tree.c -Wno-deprecated-declarations
