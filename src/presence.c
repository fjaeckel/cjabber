#include <common.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void presence(netdata *net){
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
  iks_send(net->parser,x);
}
