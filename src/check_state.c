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
#include <common.h>

int check_state(int state){
  switch (state) {
    case IKS_OK:
      puts("OK");
      return 0;
    case IKS_NET_NODNS:
      if(error("hostname lookup failed") == 1)
        return 1;
    case IKS_NET_NOSOCK:
      if(error("socket cannot be created") == 1)
        return 1;
    case IKS_NET_NOCONN:
      if(error("connection failed") == 1)
        return 1;
    default:
      if(error("io error") == 1)
        return 1;
  }
  return 0;
}
