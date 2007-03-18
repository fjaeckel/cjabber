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
#include <common.h>

/* roster */
iks *cj_roster;

/* a hook on error */
int on_error (void *user_data, ikspak *pak){
  /* FIXME: uh, ok.. its a hook.. but maybe it works with error()? */
	error ("authorization failed");
	return IKS_FILTER_EAT;
}

/* a hook for the roster */
int on_roster (netdata *net, ikspak *pak){
	cj_roster = pak->x;
  /* FIXME: do we really need a job done flag? */
	net->job_done = 1;
	return IKS_FILTER_EAT;
}

/* a hook, for the results */
int on_result (netdata *net, ikspak *pak){
	iks *x;
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
