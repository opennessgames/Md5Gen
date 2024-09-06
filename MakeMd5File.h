/*
 *    FileName : MakeMd5File.h
 *    By : yzx
 *    MakeTime : 2024年9月4日 下午 08:09:08
 *    Copyright (C) 2023-2024 openGames
 */

#ifndef MAKEMD5FILE_H
#define MAKEMD5FILE_H

#include <stdbool.h>
#include "modules/cJSON.h"

typedef enum
{
    MD5_SUM = 0,
    SHA1_SUM,
    SHA256_SUM,
    CRC32_SUM,
} Hash; // 哈希值类型
typedef struct md5gen
{
    unsigned char MD5;
    unsigned char SHA1;
    unsigned char SHA256;
    unsigned char CRC32;
} SHSH;

bool md5gen(cJSON *, char *, const char *, int); /* 生成Md5文件 */
#endif                                           // MAKEMD5FILE_H