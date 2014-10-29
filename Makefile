NAME=libbpcrypt
UNAME_OS=$(shell uname -s)

all: shared

shared: crypt.o tree.o
ifeq ($(UNAME_OS), Darwin)
	gcc -dynamiclib -o $(NAME).dylib -lssl -lcrypto crypt.c tree.c -Wno-deprecated-declarations
else
	gcc -shared -o $(NAME).so crypt.o tree.o -lssl -lcrypto -g
endif

static:
	gcc -c crypt.c
	gcc -c tree.c
	ar rs $(NAME).a crypt.o tree.o

clean:
	rm -rf *.so *.a *.o *.dylib test-crypt test-tree *.dSYM

crypt.o:
	gcc -c -fPIC crypt.c -g -Wno-deprecated-declarations

tree.o:
	gcc -c -fPIC tree.c -g -Wno-deprecated-declarations

tests: shared
	gcc -I. -L. -o test-crypt test-crypt.c -lbpcrypt -lcrypto -g -Wno-deprecated-declarations
ifeq ($(UNAME_OS), Darwin)
	gcc -I. -L. -o test-tree test-tree-macos.c -lbpcrypt -g
else
	gcc -I. -L. -o test-tree test-tree.c -lbpcrypt -g
endif

macosx:
	clang -dynamiclib -o libbpcrypt.dylib -lssl -lcrypto crypt.c tree.c -Wno-deprecated-declarations
	clang -I. -L. -o test-crypt libbpcrypt.dylib test-crypt.c -lcrypto -Wno-deprecated-declarations
	clang -I. -L. -o test-tree libbpcrypt.dylib test-tree.c -Wno-deprecated-declarations

#	gcc -c -fPIC crypt.c -g  -Wno-deprecated-declarations
#	gcc -c -fPIC tree.c -g -Wno-deprecated-declarations
#	gcc -dynamiclib -undefined suppress -flat_namespace crypt.o tree.o -o libbpcrypt.dylib -lcrypto -lssl -g -Wno-deprecated-declarations
#	gcc -I. -L. -o test-tree test-tree.cpp libbpcrypt.dylib -Wno-deprecated-declarations -g -lc++


