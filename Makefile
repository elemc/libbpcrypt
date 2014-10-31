NAME=libbpcrypt
UNAME_OS=$(shell uname -s)
CC=cc

all: shared

shared: crypt.o tree.o misc.o old_crypt.o
ifeq ($(UNAME_OS), Darwin)
	$(CC) -dynamiclib -o $(NAME).dylib -lssl -lcrypto crypt.o tree.o misc.o old_crypt.o -Wno-deprecated-declarations
else
	$(CC) -shared -o $(NAME).so crypt.o tree.o misc.o old_crypt.o -lssl -lcrypto -g
endif

static:
	$(CC) -c crypt.c
	$(CC) -c tree.c
	ar rs $(NAME).a crypt.o tree.o

clean:
	rm -rf *.so *.a *.o *.dylib test-crypt test-tree test-misc test-old-crypt default.xml *.dSYM

crypt.o:
	$(CC) -c -fPIC crypt.c -g -Wno-deprecated-declarations

tree.o:
	$(CC) -c -fPIC tree.c -g -Wno-deprecated-declarations

misc.o:
	$(CC) -c -fPIC misc.c -g -Wno-deprecated-declarations

old_crypt.o:
	$(CC) -c -fPIC old_crypt.c -g -Wno-deprecated-declarations


tests: shared
	$(CC) -I. -L. -o test-crypt test-crypt.c -lbpcrypt -lcrypto -g -Wno-deprecated-declarations
	$(CC) -I. -L. -o test-tree test-tree.c -lbpcrypt -g
	$(CC) -I. -L. -o test-misc test-misc.c -lbpcrypt -g
	$(CC) -I. -L. -o test-old-crypt test-old-crypt.c -lbpcrypt -g

macosx:
	clang -dynamiclib -o libbpcrypt.dylib -lssl -lcrypto crypt.c tree.c -Wno-deprecated-declarations
	clang -I. -L. -o test-crypt libbpcrypt.dylib test-crypt.c -lcrypto -Wno-deprecated-declarations
	clang -I. -L. -o test-tree libbpcrypt.dylib test-tree.c -Wno-deprecated-declarations

#	$(CC) -c -fPIC crypt.c -g  -Wno-deprecated-declarations
#	$(CC) -c -fPIC tree.c -g -Wno-deprecated-declarations
#	$(CC) -dynamiclib -undefined suppress -flat_namespace crypt.o tree.o -o libbpcrypt.dylib -lcrypto -lssl -g -Wno-deprecated-declarations
#	$(CC) -I. -L. -o test-tree test-tree.cpp libbpcrypt.dylib -Wno-deprecated-declarations -g -lc++


