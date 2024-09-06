/*
 *    FileName : GetMd5File.c
 *    By : yzx
 *    MakeTime : 2024年9月5日 下午 05:42:17
 *    Copyright (C) 2023-2024 openGames
 */
#ifndef GETMD5FILE_H
#define GETMD5FILE_H
#include "modules/cJSON.h"
typedef struct
{
    unsigned int valid;
    unsigned int invalid;
    unsigned int missing;
    unsigned int all;
} Summarize;

/*
    valid - 通过
    invalid - 不通过
    missing - 文件丢失
*/
char *concatenate(const char *, const char *, const char *); /* 三个字符串拼接 */
int compare_hashes(const char *, cJSON *);                   /* 比较哈希值 */
int count_filtered_objects(const cJSON *, const char *);     /* 统计对象 */
void traverse_directory(const char *, cJSON *);              /* 递归遍历文件夹 */
void md5gen_get(char *, const char *);

#endif // GETMD5FILE_H