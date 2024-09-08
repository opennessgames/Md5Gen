/*
 *    FileName : sha256.c
 *    By : yzx
 *    MakeTime : 2024年9月5日 下午 06:12:24
 *    Copyright (C) 2023-2024 openGames
 */

// 设置读取大文件所需的宏
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include "sha256.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static const char HELP_INFO[] = "Use command 'sha256 <file1> [file2 [file3 ...]]' to calculate sha256 value.\n";

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("%s", HELP_INFO);
        return 0;
    }

    struct stat file_info;
    unsigned long long BUFFER_SIZE;
    unsigned char *buffer;
    unsigned char *file_content;
    unsigned char sha256_result[32] = {0};

    // 设置标准输出和标准错误为无缓冲
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    // 对文件逐个计算sha256
    for (int i = 1; i < argc; i++)
    {
        // 打开文件
        FILE *file_stream = fopen(argv[i], "rb");
        if (file_stream == NULL)
        {
            fprintf(stderr, "Couldn't open file '%s'!\n", argv[i]);
            fclose(file_stream);
            continue;
        }

        // 获取文件大小
        if (fstat(fileno(file_stream), &file_info) != 0)
        {
            fprintf(stderr, "Couldn't get info for file '%s'!\n", argv[i]);
            fclose(file_stream);
            continue;
        }
        unsigned long long file_size = (unsigned long long)file_info.st_size;

        // 文件小于1M,1G,1T时，分别设置缓冲区
        if (file_size < 1048576ull)
        {
            BUFFER_SIZE = 1024ull;
        }
        else if (file_size < 1073741824ull)
        {
            BUFFER_SIZE = 1048576ull;
        }
        else if (file_size < 1099511627776ull)
        {
            BUFFER_SIZE = 1073741824ull;
        }
        else
        {
            fprintf(stderr, "file '%s' is to big!\n", argv[i]);
            fclose(file_stream);
            continue;
        }

        if ((buffer = malloc(BUFFER_SIZE)) == NULL)
        {
            fputs("Insufficient memory space!\n", stderr);
            fclose(file_stream);
            continue;
        }
        if (setvbuf(file_stream, buffer, _IOFBF, BUFFER_SIZE) != 0)
        {
            fprintf(stderr, "Couldn't set read buffer for file '%s'!\n", argv[i]);
            fclose(file_stream);
            free(buffer);
            continue;
        }

        // 读取文件内容
        if ((file_content = malloc(file_size)) == NULL)
        { // 防止栈空间溢出，使用malloc
            fputs("Insufficient memory space!", stderr);
            fclose(file_stream);
            free(buffer);
            continue;
        }
        fread(file_content, 1, file_size, file_stream);
        fclose(file_stream);

        // 计算结果
        if (sha256(file_content, file_size, sha256_result))
        {
            for (int i = 0; i < 32; i++)
            {
                printf("%02x", sha256_result[i]);
            }
            printf("  <file: %s>  <size: %llu Bytes>\n", argv[i], file_size);
        }
        else
        {
            fprintf(stderr, "Error occur in calculating hash for file '%s'!\n", argv[i]);
        }

        // 释放空间
        free(buffer);
    }

    return 0;
}