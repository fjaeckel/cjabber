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
#include <unistd.h>
#include <errno.h>
#include <error.h>

/*
 * a simple error function..
 * returns '1' if something is going wrong.
 */
int error(char *msg) {
  printf("error: ");
  puts(msg);
  return 1;
}

int check_errno(int errornumber) {
  printf("errno: %d\n",errornumber);
  switch(errornumber) {
    case EAGAIN:
        write(STDOUT_FILENO,ERR_AGAIN,sizeof(ERR_AGAIN));
        return 1;
        break;
    case EBADF:
        write(STDOUT_FILENO,ERR_BADF,sizeof(ERR_BADF));
        return 1;
        break;
    case EFAULT:
        write(STDOUT_FILENO,ERR_FAULT,sizeof(ERR_FAULT));
        return 1;
        break;
    case EINTR:
        write(STDOUT_FILENO,ERR_INTR,sizeof(ERR_INTR));
        return 1;
        break;
    case EINVAL:
        write(STDOUT_FILENO,ERR_INVAL,sizeof(ERR_INVAL));
        return 1;
        break;
    case EIO:
        write(STDOUT_FILENO,ERR_IO,sizeof(ERR_IO));
        return 1;
        break;
    case EISDIR:
        write(STDOUT_FILENO,ERR_ISDIR,sizeof(ERR_ISDIR));
        return 1;
        break;
    default:
        write(STDOUT_FILENO,"WAH! SOMETHING WEIRD HAPPENED!",32);
        return 1;
  }
  return 0;
}
