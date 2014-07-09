NAME=libbpcrypt

all: shared

shared: libbpcrypt.o
	gcc -shared -o $(NAME).so libbpcrypt.o -lssl -g

static:
	gcc -c libbpcrypt.c
	ar rs $(NAME).a libbpcrypt.o

clean:
	rm -rf libbpcrypt.so libbpcrypt.a libbpcrypt.o test libbpcrypt.dylib

libbpcrypt.o:
	gcc -c -fPIC libbpcrypt.c -g

test: shared
	gcc -I. -L. -o test test.c -lbpcrypt -g

macosx:
	clang -dynamiclib -o libbpcrypt.dylib -lssl -lcrypto libbpcrypt.c -Wno-deprecated-declarations
	clang -I. -L. -o test libbpcrypt.dylib test.c	-Wno-deprecated-declarations
