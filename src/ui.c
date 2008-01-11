#include <curses.h>
#include <string.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);

WINDOW *create_newwin(int height, int width, int starty, int startx){
  WINDOW *local_win;
  local_win = newwin(height, width, starty, startx);
  box(local_win, 0 , 0);          /* 0, 0 gives default characters 
                                   * for the vertical and horizontal
                                   * lines                        */
  wborder(local_win, '#', '#', '#', '#', '/', '\\', '\\', '/');
  //print_in_middle(local_win, (LINES-5) / 2, -2, 0, "Welcome to cjabber 0.000001 Alpha Beta!");
  wrefresh(local_win);            /* Show that box                */
  return local_win;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string) {
  int length, x, y;
  float temp;
 
  if(win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if(startx != 0)
    x = startx;
  if(starty != 0)
    y = starty;
  if(width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length)/ 2;
  x = startx + (int)temp;
  mvwprintw(win, y, x, "%s", string);
  refresh();
}


void init_ui(char *pname){
  WINDOW *win;
  WINDOW *win2;
  WINDOW *win3;
  WINDOW *win4;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  init_color(COLOR_RED, 700, 0, 0);
  win=create_newwin(1, COLS, 0 , 0);
  win2=create_newwin(LINES-3, COLS, 1, 0);
  win3=create_newwin(1, COLS, LINES-2, 0);
  win4=create_newwin(1, COLS, LINES-1, 0);
  wborder(win3, '-', '-', '-', '-', '-', '-', '-', '-');
  wrefresh(win3);
  wborder(win4, '|', '|', '-', '-', '+', '+', '+', '+');
  wrefresh(win4);
  refresh();
  getch();
  endwin();
}
