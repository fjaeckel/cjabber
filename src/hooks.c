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

#include <iksemel.h>

/*
 * a tag hook, which is present for the parser 
 * for now, it does nothing.
 */
int tagHook() {
  return IKS_OK;
}

/*
 * same as the tag hook, but for the cdata. 
 * for now, it does nothing.
 */
int cdataHook() {
  return IKS_OK;
}
