#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

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

  write(STDOUT_FILENO,text,strlen(text));
  while((ret = read(STDIN_FILENO,buf,BUFSIZE))){ 
    if(ret == -1) {     /* if ret is -1 something failed. */
      switch(errno) {
          case EAGAIN:
              write(STDOUT_FILENO,"O_NONBLOCK specified..",23);
              break;
          case EBADF:
              write(STDOUT_FILENO,"fd is not a valid filedescriptor",33);
              break;
          case EFAULT:
              write(STDOUT_FILENO,"buf is outside our accessible address space.",44);
              break;
          case EINTR:
              write(STDOUT_FILENO,"The call was interrupted by a signal before any data was read.",62);
              break;
          case EINVAL:
              write(STDOUT_FILENO,"opened with O_DIRECT, address is specified in buf or fd is attached to an unsuitable object.",92);
              break;
          case EIO:
              write(STDOUT_FILENO,"I/O Error",9);
              break;
          case EISDIR:
              write(STDOUT_FILENO,"fd refers to a directory",24);
              break;
          default:
              write(STDOUT_FILENO,"WAH! SOMETHING WEIRD HAPPENED!\0",32);
              break;
      }
      break;
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
    printf("p: %d buf: %d buf: %s\n", strlen(p),strlen(buf),buf);
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
