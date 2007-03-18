#include <iksemel.h>
#include <common.h>

//iksfilter *cj_filter;
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
