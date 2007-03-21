/*
 * cjabber - a full featured console based jabber/xmpp client written in C
 *
 * author:  Frederic 'jchome' Jaeckel
 * email:   jaeckelf 'at' refuzed 'dot' org
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

#ifdef DEBUG
  #include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <common.h>

int main(int argc, char *argv[]) {
  char *jabberid = NULL;
  char *resource = NULL;
  char *password = NULL;
  int args;
  int port = 0;
  if(argc > 2) {      
    for (args=1; args < argc; args++) {
      if(!strncmp(argv[args], "-u", sizeof(argv[args]))) {
        jabberid = argv[args+1];
        args++;
#ifdef DEBUG
        puts((char *) jabberid);
#endif
      }
      else {
        if(!strncmp(argv[args], "-p", sizeof(argv[args]))) {
         	port = atoi(argv[args+1]);
          args++;
         	if(port == 0)
         	  if(error("can't bind to this port"))
        	    return 1;
#ifdef DEBUG
        	printf("port: %i\n", port);
#endif
        }
        if(!strncmp(argv[args],"-r",sizeof(argv[args]))){
          resource = argv[args+1];
          args++;
#ifdef DEBUG
          printf("resource: %s\n",resource);
#endif
        }
        if(!strncmp(argv[args],"-pw",sizeof(argv[args]))){
            password=argv[args+1];
            args++;
#ifdef DEBUG
            printf("password: %s\n",password);
#endif
        }
      }
    }
  }

  /*
   * connect, d00d! :-D
   */
  cj_connect(jabberid,password,resource,port,0);
  return 0;
}
