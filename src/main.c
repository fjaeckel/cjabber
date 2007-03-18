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

/* roster */
iks *cj_roster;
//iksfilter *cj_filter;
/* a hook on error */
int on_error (void *user_data, ikspak *pak){
  printf("foo");
  /* FIXME: uh, ok.. its a hook.. but maybe it works with error()? */
	error ("authorization failed");
	return IKS_FILTER_EAT;
}

/* a hook for the roster */
int on_roster (netdata *net, ikspak *pak){
  printf("on roster! :-P\n");
	cj_roster = pak->x;
  /* FIXME: do we really need a job done flag? */
	net->job_done = 1;
	return IKS_FILTER_EAT;
}

/* a hook, for the results */
int on_result (netdata *net, ikspak *pak){
	iks *x;
  printf("on result! :-P\n");
  /* FIXME: set_roster? */
	if (net->set_roster == 0){
		x = iks_make_iq (IKS_TYPE_GET, IKS_NS_ROSTER);
		iks_insert_attrib (x, "id", "roster");
		iks_send (net->parser, x);
		iks_delete (x);
	} else {
		iks_insert_attrib (cj_roster, "type", "set");
		iks_send (net->parser, cj_roster);
	}
	return IKS_FILTER_EAT;
}

void on_log (netdata *net, const char *data, size_t size, int is_incoming) {
	if (iks_is_secure (net->parser)) fprintf (stderr, "Sec\n");
	if (is_incoming) fprintf (stderr, "<< RECV << \n"); else fprintf (stderr, ">> SEND >> \n");
	fprintf (stderr, "[%s]\n", data);
}

/*
 * the connection routine for connecting to the server,
 * holding the connection, sending things and 
 * authentification.
 */
int cj_connect(char *jabberid, char *pass, char *resource, int port, int set_roster) {
  int state;
  netdata net;
  /* create a new stream on the parser */
  net.parser = iks_stream_new(IKS_NS_CLIENT, &net, (iksStreamHook *) cj_stream);
#ifdef DEBUG
  iks_set_log_hook (net.parser, (iksLogHook *) on_log);
  if(net.parser)
    puts("net.parser initiated.");
#endif
  
  if(pass)
      net.password = pass;
  else
      error("no password set.");
  net.set_roster = set_roster;

  /* copy the new jabberid to the net struct */
  net.id = (iksid*) create_id(jabberid, &net);

  /* setting the resource */
  if(!net.id->resource && !resource)
    net.id->resource = RESOURCE;
  else
      net.id->resource = resource;

  /* just a boring message.. ;-) */
  printf("Connecting to '%s'...", net.id->server);
  
  /* setup the filter for the auth! */
  setup_filter (&net);
  /* try to connect to the remote server */
  if(port == 0 && !port)
    state = iks_connect_tcp(net.parser, net.id->server, IKS_JABBER_PORT);
  else
    state = iks_connect_tcp(net.parser, net.id->server, port);

  /* check wether the connection is established or not. */
  if (check_state(state)){
      error("something with the connection went wrong");
      return 1;
  }

#ifdef DEBUG
  printf("username: %s\nserver: %s\nresource: %s\npartial: %s\nfull: %s\n",
    net.id->user, net.id->server, net.id->resource, net.id->partial,
    net.id->full);
#endif

  net.counter = TIMEOUT;

  while (1) {
		state = iks_recv (net.parser, 1);
		if (IKS_HOOK == state)
        break;
		if (IKS_NET_TLSFAIL == state)
        error ("tls handshake failed");
		if (IKS_OK != state)
        error ("io error");
    net.counter--;
    if (net.counter == 0) {
        error ("network timeout");
        break;
    }
	}
  /* disconnects the parser. */
//  iks_disconnect(net.parser);
  /* deletes the parser.. */
  iks_parser_delete(net.parser);
  return 0;
}

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
        if(!strncmp(argv[args],"-pw",sizeof(argv[args]))){
            password=argv[args+1];
#ifdef DEBUG
            printf("password: %s\n",password);
#endif
        }
      }
    }
  }

  cj_connect(jabberid,password,resource,port,0);
  return 0;
}
