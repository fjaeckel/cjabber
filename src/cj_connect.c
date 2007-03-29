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

#include <common.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
 * the connection routine for connecting to the server,
 * holding the connection, sending things and 
 * authentification.
 */
int cj_connect(char *jabberid, char *pass, char *resource, int port, int set_roster) {
  int state=0;
  netdata net;
  /* create a new stream on the parser */
  net.parser = iks_stream_new(IKS_NS_CLIENT, &net, (iksStreamHook *) cj_stream);
#ifdef DEBUG
  iks_set_log_hook (net.parser, (iksLogHook *) on_log);
  if(net.parser)
    puts("net.parser initiated.");
#endif
  
  /* 
   * yes, for now I forbid passwords <=1 character. 
   */
  if(pass && strlen(pass)>1)
    net.password = pass;
  else {
    /*
     * FIXME: I WANT TO BE FIXED!!!111oneoneeleven
     *
     * passwords >127 chars aren't practiable.
     * maybe I'll change it to a dynamic pointer
     * but for now I'm too lazy. :-P
     */
    char password[128]= {""};
    printf("Enter your password: ");
    fgets(password,127,stdin);
    strtok(password,"\r\n");
    printf("%d\n",strlen(password));
    if (strlen(password)>1) {
      net.password = password;
      printf("password: %s\n",net.password);
    }
    else {
      error("no or too short password specified");
      return 1;
    }
  }
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
		if (state == IKS_HOOK)
      break;
		if (state == IKS_NET_TLSFAIL) {
      error ("tls handshake failed");
      return 1;
    }
		if (state != IKS_OK) {
      error ("io error");
      return 1;
    }
    net.counter--;
    if (net.counter == 0) {
      error ("network timeout");
      return 1;
    }
	}
  presence(&net);
  /*/while(net.parser){
    iks_recv(net.parser,30);
  }*/
  /* just to see the presence in another roster */
//  sleep(15);
  
  /* disconnects the parser. */
  printf("disconnecting...");
  iks_disconnect(net.parser);
  puts("OK");
  /* deletes the parser.. */
  printf("deleting parser...");
  iks_parser_delete(net.parser);
  puts("OK");
  return 0;
}
