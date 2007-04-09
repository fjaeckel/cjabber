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
  int ret=0;          /* the return code of read() */
  int cnt=0;          /* the count for allocating the memory */
  char buf[BUFSIZE];  /* our buffer, BUFSIZE big */
  char *q=NULL;       /* a backup pointer if realloc() fails */
  int errornumber=0;

  write(STDOUT_FILENO,text,strlen(text));
  while((ret = read(STDIN_FILENO,buf,BUFSIZE)) && buf[cnt-1] != '\n'){
    if(ret == -1) {     /* if ret is -1 something is wrong. */
        printf("errno: %d\n",errno);
        errornumber = errno;
        if (check_errno(errornumber) != 0)
            return NULL;
    }
    cnt+=ret;         /* increment cnt with the content of ret */
    q=p;              /* assign the backup pointer */
    if(buf[cnt] == '\n') {
        p=realloc(p,cnt-1);
        break;
    }
    else
        p=realloc(p,cnt); /* reallocate the pointer to a fitting memrange */
    if(p == NULL){     /* if NULL, then it's not allocateable */
        free(q);
        break;
    }
    strcat(p,buf);    /* append the buf content to p */
#ifdef DEBUG
    printf("p: %d buf: %d cnt %d buf: %s\n", strlen(p),strlen(buf),cnt,buf);
#endif
    if(buf[cnt-1]=='\n')
        break;
  }
#ifdef DEBUG
  printf("buf: %d p: %d\n",strlen(buf),strlen(p));
#endif
  strtok(p,"\r\n");   /* we don't want newlines at the end. */
#ifdef DEBUG
  printf("%s%s\n",text,p);
#endif
  flush_stdin();      /* flushing the buffer rockz! */
  return p;           /* return the pointer to the string now */
}
