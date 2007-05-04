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

typedef struct winsize wsize;

void create_swindow(int cols, int row) {
  int columns=0;
  for (columns=cols; columns>0; columns--){
      write(0,"-",1);
  }
}

wsize get_termsize() {
  wsize size;
  ioctl(0, TIOCGWINSZ, (char *) &size);
  return size;
}

void ui(netdata *net) {
  wsize size;
  size = get_termsize();
  menu(net);
  create_swindow(size.ws_col,size.ws_row);    
}

void create_msg(netdata *net) {
}

void show_roster(netdata *net) {
    iks *x=NULL;
    x=iks_make_iq(IKS_TYPE_GET,"jabber:iq:roster");
    iks_send(net->parser,x);
    iks_recv(net->parser,10);
}

void menu(netdata *net) {
//  char *myptr=NULL;
  char buf[2];
/*  char *endptr=NULL;
  long val=0;
  while (val != 9) {*/
  /*  myptr=enter_text("[1] send a message\n"
                   "[2] show the roster\n"
                   "[3] set your presence state\n"
                   "[9] quit\n"
                   "----------------------------\n"
                   "your choice: ",myptr);
    printf("%s\n",myptr);*/
/*    val=strtol(ptr,&endptr,10);
    printf("%s, %ld\n",ptr,val);
    
   // if (ptr!=NULL) free(ptr);*/
  while (atoi(buf) != 9) {
    flush_stdin();
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
//  if (ptr!=NULL)
//      free(ptr);
//}


