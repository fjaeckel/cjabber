#	just a Makefile to compile and link cjabber.
#	
#	This file is part of cjabber.
#
#	cjabber is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License version 2
#	as published by the Free Software Foundation.
#	
#	cjabber is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#	GNU General Public License for more details.
#	
#	You should have received a copy of the GNU General Public License
#	along with cjabber; if not, write to the Free Software
#	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

CC=gcc
SRC=src
LIBS=-lcurses
#CFLAGS=-Wall -DDEBUG -Iinclude -ggdb3 -g
CFLAGS=-Wall -DDEBUG -Iinclude 
#CFLAGS=-Wall -Iinclude
OBJ=ui.o


all: cjabber

cjabber: ${OBJ} main.o
	${CC} -o cjabber ${SRC}/*.o ${LIBS} ${CFLAGS}

ui.o:
	${CC} -c ${SRC}/ui.c -o ${SRC}/ui.o ${CFLAGS}

main.o: ${OBJ}
	${CC} -c ${SRC}/main.c -o ${SRC}/main.o ${CFLAGS}

clean:
	rm cjabber ${SRC}/*.o
