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

#include <iksemel.h>
#include <netdata.h>

#define RESOURCE "anyjabberclient"
#define JABBERID "jabber@localhost"
#define TIMEOUT 10

void usage(char *pname);
int cj_stream();
int cdataHook();
int error(char *msg);
int check_state(int state);
iksid *create_id(char *jabberid, netdata *net);
iksfilter *cj_filter;
int on_error(void *user_data, ikspak *pak);
int on_roster(netdata *net, ikspak *pak);
int on_result(netdata *net, ikspak *pak);
void setup_filter(netdata *net);
int cj_connect(char *jabberid, char *pass, char *resource, int port, int set_roster);
