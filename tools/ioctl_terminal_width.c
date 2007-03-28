#include <stdio.h>
#include <sys/ioctl.h>

int main() {
  struct winsize size;
  ioctl(0, TIOCGWINSZ, (char *) &size);
  printf("cols: %d rows: %d\n",size.ws_col,size.ws_row);
  return 0;
}
