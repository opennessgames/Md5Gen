/*
 *    FileName : testmd5.c
 *    By : yzx
 *    MakeTime : 2024年9月5日 下午 05:51:41
 *    Copyright (C) 2023-2024 openGames
 */
#include <stdio.h>
#include <stdlib.h>
#include "md5.h"
#include <string.h>
// 字符串
void main(void)
{
    int read_len;
    int i;
    char temp[8] = {0};
    unsigned char digest[16]; // 存放结果
    char hexbuf[128] = "12334567";
    unsigned char decrypt[16] = {0};
    unsigned char decrypt32[64] = {0};

    // md5:32135a337f8dc8e2bb9a9b80d86bdfd0

    MD5_CTX md5c;

    MD5Init(&md5c); // 初始化
    read_len = strlen(hexbuf);
    MD5Update(&md5c, (unsigned char *)hexbuf, read_len);

    MD5Final(&md5c, decrypt);
    strcpy((char *)decrypt32, "");

    for (i = 0; i < 16; i++)
    {
        sprintf(temp, "%02x", decrypt[i]);
        strcat((char *)decrypt32, temp);
    }
    printf("md5:%s\n", decrypt32);

    return;
}