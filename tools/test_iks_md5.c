#include <stdio.h>
#include <iksemel.h>
#include <string.h>

int main () { 
    unsigned char *md5;
    char *pass;
    puts("foo");
    pass = "foothebar";
    puts("foo");
    iks_md5(pass,md5);
    printf("%s\n",md5);
    return 0;
}
