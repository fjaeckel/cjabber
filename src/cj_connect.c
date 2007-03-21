#include <common.h>
#include <stdio.h>

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
  else {
      char password[128];
      printf("Enter your password: ");
      fgets(password,127,stdin);
      strtok(password,"\r\n");
      if (password) {
        net.password = password;
        printf("password: %s\n",net.password);
      }
      else {
          error("no password specified");
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
		if (IKS_HOOK == state)
        break;
		if (IKS_NET_TLSFAIL == state) {
        error ("tls handshake failed");
        return 1;
    }
		if (IKS_OK != state) {
        error ("io error");
        return 1;
    }
    net.counter--;
    if (net.counter == 0) {
        error ("network timeout");
        return 1;
    }
	}
  iks *x;
  printf("[1] free for chat\n"
         "[2] away\n"
         "[3] extended away\n"
         "[4] dnd\n"
         "[0] unavailable\n"
         "----------------\n"
         "select your presence state: ");
  char buf[2];
  fgets(buf,2,stdin);
  strtok(buf,"\r\n");
  switch(atoi(buf)) {
      case 1:
          x = iks_make_pres(IKS_SHOW_CHAT,"");
          break;
      case 2:
          x = iks_make_pres(IKS_SHOW_AWAY,"");
          break;
      case 3:
          x = iks_make_pres(IKS_SHOW_AWAY,"i'm just fucking away, d00d!");
          break;
      case 4:
          x = iks_make_pres(IKS_SHOW_DND,"");
          break;
      case 5:
          x = iks_make_pres(IKS_SHOW_UNAVAILABLE,"");
          break;
      default:
          break;
  }
  iks_send(net.parser,x);
  /*/while(net.parser){
    iks_recv(net.parser,30);
  }*/
  /* just to see the presence in another roster */
  sleep(15);
  
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
