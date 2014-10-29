NAME=libbpcrypt

all: shared

shared: crypt.o tree.o
	gcc -shared -o $(NAME).so crypt.o tree.o -lssl -g

static:
	gcc -c crypt.c
	gcc -c tree.c
	ar rs $(NAME).a crypt.o tree.o

clean:
	rm -rf *.so *.a *.o *.dylib test test-tree

crypt.o:
	gcc -c -fPIC crypt.c -g

tree.o:
	gcc -c -fPIC tree.c -g

test: shared
	gcc -I. -L. -o test test.c -lbpcrypt -lcrypto -g
	gcc -I. -L. -o test-tree test-tree.c -lbpcrypt -g

macosx:
	clang -dynamiclib -o libbpcrypt.dylib -lssl -lcrypto libbpcrypt.c libbpcrypt_tree.c -Wno-deprecated-declarations
	clang -I. -L. -o test libbpcrypt.dylib test.c -lcrypto -Wno-deprecated-declarations
	clang -I. -L. -o test-tree libbpcrypt.dylib test-tree.c -Wno-deprecated-declarations
