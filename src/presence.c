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
#include <stdlib.h>

void set_presence_offline(netdata *net){
  iks *x=NULL;
  x=iks_make_pres(IKS_SHOW_UNAVAILABLE,"");
  if(x!=NULL){
      iks_send(net->parser,x);
  }
}

void presence(netdata *net){
  iks *x=NULL;
  
  /*
   * dirty code, but efficient code.
   * stfu if you don't like it.. i'll fix it in the future.
   */
  flush_stdin();
  char buf[2];
  printf("[1] free for chat\n"
         "[2] away\n"
         "[3] extended away\n"
         "[4] dnd\n"
         "[5] unavailable\n------------------\n"
         "select your presence state: ");
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
  if(x!=NULL) {
    iks_send(net->parser,x);
  }
  /*
   * FIXME:
   * if you set the presence state and request the roster later
   * on, you'll get the presence states of your roster participants.
   * This is not, what I want. I'll have to fix this, with a weird
   * iks_recv() until there is no presence message left or something
   * weird like this. Maybe I'll use this information to represent the
   * existing roster in the UI.
   */
}
