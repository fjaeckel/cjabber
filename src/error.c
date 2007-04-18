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
#include <unistd.h>
#include <errno.h>

#define WEIRDERROR  "Something weird happened. RTFS.\n"

/*
 * a simple error function..
 * returns '1' if something is going wrong.
 */
int error(char *msg) {
  printf("error: ");
  puts(msg);
  return 1;
}


/*
 * this function checks errno and prints the 
 * description for it on stderr/stdout.
 */
int check_errno(int errornumber) {
  char buf[256];
#ifdef DEBUG
  printf("errno: %d\n",errornumber);
#endif
  if (errornumber>0){
      strerror_r(errornumber,buf,sizeof(buf));
    write(STDOUT_FILENO,buf,strlen(buf));
    write(STDOUT_FILENO,"\n",1);
    return 1;
  }
  else {
      write(STDOUT_FILENO,WEIRDERROR,strlen(WEIRDERROR));
      return 1;
  }
  return 0;
}
