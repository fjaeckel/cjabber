/*
 * A basic iksemel testfile..
 * it just creates an xml tree and prints it.
 * never mind.
 * compile it like:
 *      gcc -o test_iks test_iks.c -liksemel
 * for the debug usage:
 *      gcc -o test_iks test_iks.c -liksemel -DDEBUG
 *
 * author:  Frederic 'jchome' Jaeckel
 * email:   jaeckelf 'at' refuzed 'dot' org
 * license: GPLv2
 */

#include <stdio.h>
#include <iksemel.h>

#define RESOURCE "anyjabberclient"
#define JABBERID "jabber@localhost"

/* the struct for all network data */
struct netdata {
    iksparser *parser;
    iksid *id;
};

/* 
 * a simple error function..
 * returns '1' if something is going wrong.
 */
int error(char *msg){
    puts(msg);
    return 1;
}

/*
 * a tag hook, which is present for the parser 
 * for now, it does nothing.
 */
int tagHook (){
    return IKS_OK;
}

/*
 * same as the tag hook, but for the cdata. 
 * for now, it does nothing.
 */
int cdataHook (){
    return IKS_OK;
}

int main () {
    iksid *myjabberid=NULL;
    int state;

    /* 
     * initialising the struct net of the type netdata 
     */
    struct netdata net;

    /* create a new SAX parser */
    net.parser = iks_sax_new(NULL, tagHook, cdataHook);
    /* check if the parser is valid */
    if (!net.parser) error("fuck off damn bastard.");
    /* create a new stream on the parser */
    net.parser = iks_stream_new(IKS_NS_CLIENT, &net, (iksStreamHook *) tagHook);

    /* just for testing purposes */
#ifdef DEBUG
    if ( net.parser ) puts("net.parser initiated.");
#endif
    
    /* create a new jabberid */
    if (!(myjabberid=iks_id_new(iks_parser_stack(net.parser),JABBERID)))
        error("cannot create uid");
    
    /* again, only for testing purposes */
#ifdef DEBUG
    printf("%s@%s\n", myjabberid->user,myjabberid->server);
#endif
   
    /* copy the new jabberid to the net struct */
    net.id = myjabberid;
    /* just a boring message.. ;-) */
    printf("Connecting to '%s'...\n", net.id->server);
    /* try to connect to the remote server */
    state=iks_connect_tcp(net.parser, net.id->server, IKS_JABBER_PORT);
    
    /*
     * check wether the connection is established or not.
     */
    switch(state){
            case IKS_OK:
                /* everything is OK! ;-) */
                break;
            case IKS_NET_NODNS:
                /* hostname could not be resolved*/
                if (error("hostname lookup failed") == 1){
                    iks_disconnect(net.parser);
                    iks_parser_delete(net.parser);
                    return 1;
                }
            case IKS_NET_NOSOCK:
                /* socket descriptor cannot be created */
                if (error("socket cannot be created") == 1){
                    iks_disconnect(net.parser);
                    iks_parser_delete(net.parser);
                    return 1;
                }
            case IKS_NET_NOCONN:
                /* 
                 * the connection cannot be established..
                 * this could have several sources.
                 */
                if (error("connection failed") == 1){
                    iks_disconnect(net.parser);
                    iks_parser_delete(net.parser);
                    return 1;
                }
            default:
                /* everything else. */
                if (error("io error") == 1){
                    iks_disconnect(net.parser);
                    iks_parser_delete(net.parser);
                    return 1;
                }
    }
    if (!net.id->resource)
        net.id->resource = RESOURCE;

#ifdef DEBUG
    printf("username: %s\nserver: %s\nresource: %s\npartial: %s\nfull: %s\n",
            net.id->user, net.id->server, net.id->resource, net.id->partial, net.id->full);
#endif
/*    iks *x;
    x = iks_make_auth(net.id, "foobar", iks_find_attrib(node, "id"));
    iks_insert_attrib(x, "id", "auth");
    iks_send(net.parser, x);
    iks_delete(x);*/

    /* disconnect the parser. */
    iks_disconnect(net.parser);
    /* delete the parser..*/
    iks_parser_delete(net.parser);
    return 0;
}
