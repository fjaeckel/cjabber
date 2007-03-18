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

#include <string.h>
#include <stdio.h>
#include <common.h>

/* roster */
iks *cj_roster;

int sasl_established;

/* a hook on error */
int on_error (void *user_data, ikspak *pak){
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

int cj_stream(netdata *net,int type, iks *node){
    switch(type) {
        case IKS_NODE_START:
            if(!iks_is_secure(net->parser)){
              iks_start_tls(net->parser);
#ifdef DEBUG
              puts("----------------------------");
              puts(">> started tls connection <<");
              puts("----------------------------");
#endif
              break;
            }
            else {
              if (sasl_established && iks_is_secure(net->parser)){
                iks *auth;
                auth = iks_make_auth(net->id, net->password, iks_find_attrib (node, "id"));
                iks_insert_attrib(auth, "id", "auth");
                iks_send(net->parser, auth);
                iks_delete(auth);
#ifdef DEBUG
                puts("--------------------------");
                puts(">>  sent a auth packet  <<");
                puts("--------------------------");
#endif
              }
            }
            break;
        case IKS_NODE_NORMAL:
            if (strcmp("stream:features",iks_name(node))==0) {
                net->features = iks_stream_features(node);
                if (!iks_is_secure(net->parser))
                    break;
#ifdef DEBUG
                printf("authorized: %d\n",net->authorized);
#endif
                if (net->authorized){
                    iks *t;
                    if (net->features & IKS_STREAM_BIND) {
                        t = iks_make_resource_bind(net->id);
                        iks_send(net->parser, t);
                        iks_delete(t);
#ifdef DEBUG
                        puts("--------------------------");
                        puts(">>    bind successful   <<");
                        puts("--------------------------");
#endif
                    }
                    if (net->features & IKS_STREAM_SESSION) {
                        t = iks_make_session();
                        iks_insert_attrib(t,"id","auth");
                        iks_send(net->parser, t);
                        iks_delete(t);
#ifdef DEBUG
                        puts("--------------------------");
                        puts(">>  session successful  <<");
                        puts("--------------------------");
#endif
                     }
                else {
                    if (net->features & IKS_STREAM_SASL_MD5){
                      iks_start_sasl(net->parser, IKS_SASL_DIGEST_MD5, net->id->user, net->password);
#ifdef DEBUG
                      puts("--------------------------");
                      puts(">>    authenticated     <<");
                      puts(">>      with MD5        <<");
                      puts("--------------------------");
#endif
                      printf("authenticating...");
                    }
                    else if (net->features & IKS_STREAM_SASL_PLAIN) {
                      iks_start_sasl(net->parser, IKS_SASL_PLAIN, net->id->user, net->password);
#ifdef DEBUG
                      puts("--------------------------");
                      puts(">>    authenticated     <<");
                      puts(">>      with PLAIN      <<");
                      puts("--------------------------");
#endif
                      printf("authenticated...");
                    }
                  }
                }
            }
            else if(strcmp("failure", iks_name(node))==0) {
              error("sasl auth failed");
              return 3;
            }
            else if(strcmp("success", iks_name(node))==0) {
              net->authorized = 1;
              iks_send_header(net->parser, net->id->server);
#ifdef DEBUG
              puts("--------------------------");
              puts(">>       success        <<");
              puts("--------------------------");
#endif
              puts("OK");
            }
            else {
              ikspak *pak;
              pak = iks_packet(node);
              iks_filter_packet(cj_filter, pak);
#ifdef DEBUG
              puts("---------------------------------------------");
              puts(">>               progress                  <<");
              puts("---------------------------------------------");
#endif
              if(net->job_done == 1)
                return IKS_HOOK;

            }
            break;
        case IKS_NODE_STOP:
            error("connection closed.");
            break;
        case IKS_NODE_ERROR:
            error("error!!2142");
            break;
    }
    return IKS_OK;
}

/*
 * same as the tag hook, but for the cdata. 
 * for now, it does nothing.
 */
int cdataHook() {
  return IKS_OK;
}
