/*
 *    FileName : sha256.c
 *    By : yzx
 *    MakeTime : 2024年9月5日 下午 06:08:07
 *    Copyright (C) 2023-2024 openGames
 */
#ifndef __CUSTOM_SHA256_H__
#define __CUSTOM_SHA256_H__

#include <stdbool.h>
// 为计算较大规模数据的哈希值，data_from_malloc参数必须是malloc或calloc申请的空间的指针，调用此函数后data_from_malloc中的数据会被回收
bool sha256(unsigned char *data_from_malloc, unsigned long long len, unsigned char *out);

#endif