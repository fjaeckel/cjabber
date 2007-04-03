#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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
    if(ret == -1)     /* if ret is -1 something failed. */
      break;
    cnt+=ret;         /* increment cnt with the content of ret */
    q=p;              /* assign the backup pointer */
    p=realloc(p,cnt); /* reallocate the pointer to a fitting memrange */
    if(p == NULL)     /* if NULL, then it's not allocateable */
      break;
    strcat(p,buf);    /* append the buf content to p */
#ifdef DEBUG
    printf("p: %d buf: %d buf: %s\n", strlen(p),strlen(buf),buf);
#endif
    if(strncmp(buf,"\n",1)) /* if buf is \n, we don't want to read anymore */
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
