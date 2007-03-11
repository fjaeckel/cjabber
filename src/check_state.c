#include <iksemel.h>

int check_state(int state){
  switch (state) {
    case IKS_OK:
      /*
       * everything is OK! ;-) 
       */
      puts("OK");
      return 0;
    case IKS_NET_NODNS:
      /*
       * hostname could not be resolved 
       */
      if(error("hostname lookup failed") == 1)
        return 1;
    case IKS_NET_NOSOCK:
      /*
       * socket descriptor cannot be created 
       */
      if(error("socket cannot be created") == 1)
        return 1;
    case IKS_NET_NOCONN:
      /*
       * the connection cannot be established..
       * this could have several sources.
       */
      if(error("connection failed") == 1)
        return 1;
    default:
      /*
       * everything else. 
       */
      if(error("io error") == 1)
        return 1;
  }
}
