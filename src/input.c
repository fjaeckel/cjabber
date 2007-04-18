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
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <common.h>

#define BUFSIZE 128

/*
 * a simple function using fcntl to flush the stdin buffer
 * to prevent unwanted inputs.
 */
void flush_stdin(void) {
    /* 
     * get filestatus flag for resuming late to the old state
     */
    int oldstat = fcntl(STDIN_FILENO, F_GETFL, 0); 
    /*
     * set the fstatus flag to O_NONBLOCK to have the correct permissions 
     */
    fcntl(STDIN_FILENO, F_SETFL, oldstat | O_NONBLOCK);
    /*
     * wait for all chars left in the buffer
     */
    while (getc(stdin)!=EOF);
    /*
     * revert the filestatus flag to the old state.
     */
    fcntl(STDIN_FILENO, F_SETFL, oldstat);
}

/*
 * a generic function to enter secure strings
 * without buffer overflows.
 */
char* enter_text(char *text, char *p) {
  int ret=0;
  size_t cnt=0;
  char buf[BUFSIZE];
  char *q=NULL;
  int errornumber=0;

  write(STDOUT_FILENO,text,strlen(text));
  while((ret = read(STDIN_FILENO,buf,BUFSIZE))){
    if(ret == -1) {     /* if ret is -1 something is wrong. */
        if (p) free(p);
#ifdef DEBUG
        printf("errno: %d\n",errno);
#endif
        flush_stdin();
        errornumber = errno;
        if (check_errno(errornumber) != 0)
            return NULL;
    }
    cnt+=ret;
    q=p;
    printf("ret: %d cnt: %d\n",ret,cnt);
    p=realloc(q,cnt+1);
    if(p == NULL){
        if (q) free(q);
        return NULL;
    }
    strncat(p,buf,cnt - strlen(p));
    if(buf[ret-1]=='\n')
        break;
  }
  p[cnt - 1] = 0;

  flush_stdin();
  return p;
}
