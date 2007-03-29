#include <curses.h>

int main () {
  int lines, cols;
  initscr ();
  endwin ();
  lines = LINES;
  cols = COLS;
  printf ("cols: %d rows: %d\n", cols, lines);
  return 0;
}
