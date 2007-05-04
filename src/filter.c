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

#include <common.h>

/*
 * a simple filter for the connection.
 */
void setup_filter (netdata *net){
  if (cj_filter) /* if the filter exists, we should delete it */
      iks_filter_delete (cj_filter);
  /* create a new one. */
  cj_filter = iks_filter_new();
  /* a filter for success */
  iks_filter_add_rule (cj_filter, (iksFilterHook *) on_result, net,
      IKS_RULE_TYPE, IKS_PAK_IQ,
      IKS_RULE_SUBTYPE, IKS_TYPE_RESULT,
      IKS_RULE_ID, "auth",
      IKS_RULE_DONE);
  /* a filter for errors */
  iks_filter_add_rule (cj_filter, on_error, net,
      IKS_RULE_TYPE, IKS_PAK_IQ,
      IKS_RULE_SUBTYPE, IKS_TYPE_ERROR,
      IKS_RULE_ID, "auth",
      IKS_RULE_DONE);
  /* a filter for the roster */
  iks_filter_add_rule (cj_filter, (iksFilterHook *) on_roster, net,
      IKS_RULE_TYPE, IKS_PAK_IQ,
      IKS_RULE_SUBTYPE, IKS_TYPE_RESULT,
      IKS_RULE_ID, "roster",
      IKS_RULE_DONE);
}
