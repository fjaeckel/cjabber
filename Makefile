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
CFLAGS=-Wall -DDEBUG -Iinclude
#CFLAGS=-Wall -Iinclude

all: cjabber

cjabber: filter.o hooks.o presence.o cj_connect.o main.o error.o create_id.o check_state.o usage.o ui.o
	${CC} -o cjabber ${SRC}/*.o ${LIBS} ${CFLAGS}
ui.o:
	${CC} -c ${SRC}/ui.c -o ${SRC}/ui.o ${CFLAGS}

filter.o:
	${CC} -c ${SRC}/filter.c -o ${SRC}/filter.o ${CFLAGS}

hooks.o: filter.o
	${CC} -c ${SRC}/hooks.c -o ${SRC}/hooks.o ${CFLAGS}

presence.o:
	${CC} -c ${SRC}/presence.c -o ${SRC}/presence.o ${CFLAGS}

cj_connect.o: presence.o
	${CC} -c ${SRC}/cj_connect.c -o ${SRC}/cj_connect.o ${CFLAGS}

error.o:
	${CC} -c ${SRC}/error.c -o ${SRC}/error.o ${CFLAGS}

create_id.o:
	${CC} -c ${SRC}/create_id.c -o ${SRC}/create_id.o ${CFLAGS}

check_state.o:
	${CC} -c ${SRC}/check_state.c -o ${SRC}/check_state.o ${CFLAGS}

usage.o:
	${CC} -c ${SRC}/usage.c -o ${SRC}/usage.o ${CFLAGS}

main.o: filter.o hooks.o presence.o cj_connect.o error.o create_id.o check_state.o usage.o ui.o
	${CC} -c ${SRC}/main.c -o ${SRC}/main.o ${CFLAGS}

clean:
	rm cjabber ${SRC}/*.o
