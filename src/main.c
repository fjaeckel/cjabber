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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iksemel.h>
#include <common.h>


int main(int argc, char *argv[]) {
  char *jabberid = NULL;
  char *resource = NULL;
  int state,args;
  int port = 0;
  if(argc > 2) {      
    for (args=1; args < argc; args++) {
      if(!strncmp(argv[args], "-u", sizeof(argv[args]))) {
        jabberid = argv[args+1];
#ifdef DEBUG
        puts((char *) jabberid);
#endif
      }
      else {
        if(!strncmp(argv[args], "-p", sizeof(argv[args]))) {
         	port = atoi(argv[args+1]);
         	if(port == 0)
         	  if(error("can't bind to this port"))
        	    return 1;
#ifdef DEBUG
        	printf("port: %i\n", port);
#endif
        }
        if(!strncmp(argv[args],"-r",sizeof(argv[args]))){
          resource = argv[args+1];
#ifdef DEBUG
          printf("resource: %s\n",resource);
#endif
        }
      }
    }
  }
  /*
   * initialising the struct net of the type netdata
   * defined in netdata.h
   */
  netdata net;
  /*
   * create a new SAX parser 
   */
  net.parser = iks_sax_new(NULL, tagHook, cdataHook);

  /*
   * check if the parser is valid 
   */
  if(!net.parser)
    error("fuck off damn bastard.");

  /*
   * create a new stream on the parser 
   */
  net.parser = iks_stream_new(IKS_NS_CLIENT, &net, (iksStreamHook *) tagHook);

#ifdef DEBUG
  if(net.parser)
    puts("net.parser initiated.");
#endif

  /*
   * copy the new jabberid to the net struct 
   */
  net.id = (iksid*) create_id(jabberid, &net);

  /*
   * just a boring message.. ;-) 
   */
  printf("Connecting to '%s'...", net.id->server);

  /*
   * try to connect to the remote server 
   */
  if(port == 0)
    state = iks_connect_tcp(net.parser, net.id->server, IKS_JABBER_PORT);
  else
    state = iks_connect_tcp(net.parser, net.id->server, port);

  /*
   * check wether the connection is established or not.
   */
  if (check_state(state) == 1){
      error("something with the connection went wrong");
      return 1;
  }
  /*
   * setting the resource 
   */
  if(!net.id->resource)
    net.id->resource = RESOURCE;

#ifdef DEBUG
  printf
    ("username: %s\nserver: %s\nresource: %s\npartial: %s\nfull: %s\n",
     net.id->user, net.id->server, net.id->resource, net.id->partial,
     net.id->full);
#endif

  /*
   * disconnects the parser. 
   */
  iks_disconnect(net.parser);
  /*
   * deletes the parser.. 
   */
  iks_parser_delete(net.parser);
  return 0;
}
