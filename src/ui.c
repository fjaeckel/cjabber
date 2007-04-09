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

typedef struct winsize wsize;

/* this will be the main ui! */
void create_swindow(int cols, int row) {
  int columns=0;
  for (columns=cols; columns>0; columns--){
      printf("-");
  }
}

wsize get_termsize() {
  wsize size;
  ioctl(0, TIOCGWINSZ, (char *) &size);
  return size;
}

void ui() {
  wsize size;
  size = get_termsize();
  create_swindow(size.ws_col,size.ws_row);    
}
