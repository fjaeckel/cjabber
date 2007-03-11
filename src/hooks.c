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
