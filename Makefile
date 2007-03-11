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
LIBS=-liksemel
CFLAGS=-Wall

all: cjabber

cjabber: hooks.o main.o error.o create_id.o check_state.o
	${CC} -o cjabber ${SRC}/hooks.o ${SRC}/create_id.o ${SRC}/check_state.o ${SRC}/error.o ${SRC}/main.o ${LIBS} ${CFLAGS}

hooks.o:
	${CC} -c ${SRC}/hooks.c -o ${SRC}/hooks.o

error.o:
	${CC} -c ${SRC}/error.c -o ${SRC}/error.o

create_id.o:
	${CC} -c ${SRC}/create_id.c -o ${SRC}/create_id.o -Iinclude -DDEBUG

check_state.o:
	${CC} -c ${SRC}/check_state.c -o ${SRC}/check_state.o -Iinclude -DDEBUG

main.o: hooks.o error.o create_id.o check_state.o
	${CC} -c ${SRC}/main.c -o ${SRC}/main.o -Iinclude -DDEBUG

clean:
	rm cjabber ${SRC}/*.o
