#include <stdio.h>
#include <string.h>
#include "sha1.h"
int main()
{
    char str[1024];
    unsigned long *mac;
    while (1)
    {
        puts("input:");
        fgets(str, sizeof(str), stdin);
        mac = sha1(str, strlen(str));
        printf("SHA1=%08X%08X%08X%08X%08X\n", mac[0], mac[1], mac[2], mac[3], mac[4]);
        // system("pause");
    }
}