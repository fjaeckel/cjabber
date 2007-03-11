/*
 * A basic iksemel testfile..
 * It connects to a server given via cmdline 
 * or per default to jabber@localhost
 *
 * never mind.
 * compile it like:
 *      gcc -o test_iks test_iks.c -liksemel
 * for the debug usage:
 *      gcc -o test_iks test_iks.c -liksemel -DDEBUG
 *
 * author:  Frederic 'jchome' Jaeckel
 * email:   jaeckelf 'at' refuzed 'dot' org
 * license: GPLv2
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iksemel.h>
#include <netdata.h>
#include <common.h>

int check_state(int state){
  switch (state) {
    case IKS_OK:
      /*
       * everything is OK! ;-) 
       */
      puts("OK");
      return 0;
    case IKS_NET_NODNS:
      /*
       * hostname could not be resolved 
       */
      if(error("hostname lookup failed") == 1)
        return 1;
    case IKS_NET_NOSOCK:
      /*
       * socket descriptor cannot be created 
       */
      if(error("socket cannot be created") == 1)
        return 1;
    case IKS_NET_NOCONN:
      /*
       * the connection cannot be established..
       * this could have several sources.
       */
      if(error("connection failed") == 1)
        return 1;
    default:
      /*
       * everything else. 
       */
      if(error("io error") == 1)
        return 1;
  }
}

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
   */
  struct netdata net;
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
  net.id = create_id(jabberid, &net);

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
   * disconnect the parser. 
   */
  iks_disconnect(net.parser);
  /*
   * delete the parser.. 
   */
  iks_parser_delete(net.parser);
  return 0;
}
