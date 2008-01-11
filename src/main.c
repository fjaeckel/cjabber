#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* own headers */
#include <ui.h>

/*
 * a simple usage information
 */
void usage(char *pname){
  printf("%s -s server -p port -u username -S (1 or 0)\n", pname);
  exit(1);
}

/*
 * TODO: Arguments
 * server, port, username, ssl, 
 */
int main(int argc, char **argv){
  extern char *optarg;
  extern int optind;
  int ch = 0;
  
  while ((ch=getopt(argc, argv, "spuS")) != -1){
    switch(ch) {
      case 's':
        puts("a server.");
        break;
      case 'p':
        puts("a port.");
        break;
      case 'u':
        puts("a username");
        break;
      case 'S':
        puts("with or without SSL!");
        break;
      default:
        usage(argv[0]);
    }
  }
 // init_ui(argv[0]);
  return 0;
}
