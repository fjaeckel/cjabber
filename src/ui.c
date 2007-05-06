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

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <common.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * A general function which draws a horizontal bar of minus
 * symbols as wide as the terminal is. The terminal size is
 * defined with the arguments cols and rows.
 */
void create_swindow(int cols, int rows) {
  int columns=0;
  for (columns=cols; columns>0; columns--){
      write(0,"-",1);
  }
}

/*
 * A function to get the actual terminal size. It returns
 * the size in a struct wsize with the members ws_col and
 * ws_row.
 */
wsize get_termsize() {
  wsize size;
  ioctl(0, TIOCGWINSZ, (char *) &size);
  return size;
}

/*
 * A commmon constructor for the userinterface.
 * Basicly it draws the menu and a horizontal bar of 
 * minus symbols.
 */
void ui(netdata *net) {
  wsize size;
  size = get_termsize();
  menu(net,size);
  create_swindow(size.ws_col,size.ws_row);    
}

/*
 * This will be the function to create messages to other roster
 * participants.
 * Actually it only sends a basic message to jabber@localhost.
 * TODO:
 *  - build flexible messages of all kind of types
 *  - interactive selection of the roster participant
 *  - interactive text input
 */
void create_msg(netdata *net) {
  iks *x=NULL;
  x=iks_make_msg(IKS_TYPE_NONE,"jabber@localhost","ya better stfu.");
  iks_send(net->parser,x);
}

/*
 * This will be the function to display the whole roster.
 * It sends a roster request to the connected server and 
 * receives the answer with all roster items from the server.
 * It will show the roster in a flexible way.
 * TODO:
 *  - receive all roster items
 *    - parse 'em
 *    - make 'em accessible
 *  - build a comfortable roster bar
 *    - roster participants have to be interactively selectable
 *    - the status of te participants have to be displayed
 *    - the subscription state have to be displayed
 *    - the ressource name have to be displayed
 */
void show_roster(netdata *net) {
  iks *x=NULL;
  x = iks_make_iq (IKS_TYPE_GET, IKS_NS_ROSTER);
  iks_insert_attrib (x, "id", "roster");
  iks_send (net->parser, x);
  iks_recv(net->parser,-1);
}

/*
 * Actually the main menu.. it will be changed in the near future
 * to be a part of the whole UI. A menu based client is not comfortable.
 * It have to be interactively but integrated in the main UI.
 */
void menu(netdata *net,wsize size) {
  char buf[2];
  while (atoi(buf) != 9) {
    flush_stdin();
    create_swindow(size.ws_col,size.ws_row);
    printf("[1] send a message\n"
           "[2] show the roster\n"
           "[3] set your presence state\n"
           "[9] quit\n"
           "----------------------------\n"
           "your choice: ");
    fgets(buf,2,stdin);
    strtok(buf,"\r\n");

    switch(atoi(buf)) {
        case 1:
            create_msg(net);
            break;
        case 2:
            show_roster(net);
            break;
        case 3:
            presence(net);
            break;
        default:
            break;
    }
  }
  set_presence_offline(net);
}
