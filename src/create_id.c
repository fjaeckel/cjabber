#ifdef DEBUG
  #include <stdio.h>
#endif

#include <iksemel.h>
#include <common.h>
#include <netdata.h>

/*
 * this function returns a jabber id and returns
 * it directly
 */
iksid *create_id(char *jabberid, struct netdata * net) {
  iksid *myjabberid = NULL;
  if(jabberid) {
    if(!(myjabberid = iks_id_new(iks_parser_stack(net->parser), jabberid)))
      error("cannot create jabberid");
  }
  else {
    /*
     * create a new jabberid 
     */
    if(!(myjabberid = iks_id_new(iks_parser_stack(net->parser), JABBERID)))
      error("cannot create jabberid");
  }
  /*
   * again, only for testing purposes 
   */
#ifdef DEBUG
  printf("%s@%s\n", myjabberid->user, myjabberid->server);
#endif
  return myjabberid;
}
