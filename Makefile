#	just a Makefile to compile and link cjabber.

CC=gcc
SRC=src
LIBS=-liksemel
CFLAGS=-Wall

all: cjabber

cjabber: hooks.o main.o error.o
	${CC} -o cjabber ${SRC}/hooks.o ${SRC}/error.o ${SRC}/main.o ${LIBS} ${CFLAGS}

hooks.o:
	${CC} -c ${SRC}/hooks.c -o ${SRC}/hooks.o

error.o:
	${CC} -c ${SRC}/error.c -o ${SRC}/error.o

main.o: hooks.o error.o
	${CC} -c ${SRC}/main.c -o ${SRC}/main.o -Iinclude -DDEBUG

clean:
	rm cjabber ${SRC}/*.o
