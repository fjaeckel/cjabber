#include <stdio.h>
#include <iksemel.h>

int pr_tag (void *udata, char *name, char **atts, int type)
{
    switch (type) {
        case IKS_OPEN:
            printf ("TAG <%s>\n", name);
            break;
        case IKS_CLOSE:
            printf ("TAG </%s>\n", name);
            break;
        case IKS_SINGLE:
            printf ("TAG <%s/>\n", name);
            break;
    }
    if (atts) {
        int i = 0;
        while (atts[i]) {
            printf ("  ATTRIB %s='%s'\n", atts[i], atts[i+1]);
            i += 2;
        }
    }
    return IKS_OK;
}

enum ikserror pr_cdata (void *udata, char *data, size_t len)
{
    int i;
    printf ("CDATA [");
    for (i = 0; i < len; i++)
        putchar (data[i]);
    printf ("]\n");
    return IKS_OK;
}

int main (int argc, char *argv[])
{
    iksparser *p;
    p = iks_sax_new (NULL, pr_tag, pr_cdata);
    switch (iks_parse (p, argv[1], 0, 1)) {
        case IKS_OK:
            puts ("OK");
            break;
        case IKS_NOMEM:
            puts ("Not enough memory");
            exit (1);
        case IKS_BADXML:
            puts ("XML document is not well-formed");
            exit (2);
        case IKS_HOOK:
            puts ("Our hooks didn't like something");
            exit (2);
    }
    iks_parser_delete (p);
    return 0;
}
