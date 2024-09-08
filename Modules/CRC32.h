/*
 *    FileName : CRC32.h
 *    By : yzx
 *    MakeTime : 2024年9月4日 下午 05:05:02
 *    Copyright (C) 2023-2024 openGames
 */

#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

#define CRC32_POLYNOMIAL 0xEDB88320 // CRC32 polynomial used for calculation

void generate_crc32_table();
char *calcCRC32(const char *);

#endif // CRC32_H