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

#define RESOURCE "cjabber"
#define JABBERID "jabber@localhost"
#define TIMEOUT 10

typedef struct winsize wsize;

void usage(char *pname);
int error(char *msg);
int check_errno(int errornumber);

int cj_stream();
iksfilter *cj_filter;
void setup_filter(netdata *net);
int cj_connect(char *jabberid, char *pass, char *resource, int port, int set_roster);

int check_state(int state);

iksid *create_id(char *jabberid, netdata *net);
void presence(netdata *net);
void set_presence_offline(netdata *net);

int on_error(void *user_data, ikspak *pak);
int on_roster(netdata *net, ikspak *pak);
int on_result(netdata *net, ikspak *pak);
void on_log (netdata *net, const char *data, size_t size, int is_incoming);

void ui(netdata *net);
char* enter_text(char *text, char *p);
void flush_stdin(void);
void menu(netdata *net,wsize size);
void create_msg(netdata *net);
void show_roster(netdata *net);
