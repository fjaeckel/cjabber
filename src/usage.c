/*
 * This file is part of cjabber.
 *
 * cjabber is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * cjabber is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cjabber; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>

void usage(char *pname){
    printf("Usage: %s [OPTIONS]\n"
           "a console based jabber client.\n\n"
           "-u\tthe jabberid which you want to use\n"
           "-p\tthe port you want to connect to\n"
           "-pw\tthe passwort you want to use in cleartext ASCII\n"
           "-r\tthe name of your resource [default: cjabber]\n",pname);
}
