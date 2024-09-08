/*
 *    FileName : HashVal.h
 *    By : yzx
 *    MakeTime : 2024年9月4日 下午 04:54:11
 *    Copyright (C) 2023-2024 openGames
 */
#ifndef HASHVAL_H
#define HASHVAL_H

/**
 * @brief 计算文件的 MD5 校验和
 *
 * @param filename 文件路径
 * @return 返回 MD5 校验和的字符串（需要调用者负责释放内存）
 */
char *calcFileMd5Sum(const char *filename);

/**
 * @brief 计算文件的 SHA-1 校验和
 *
 * @param filename 文件路径
 * @return 返回 SHA-1 校验和的字符串（需要调用者负责释放内存）
 */
char *calcFileSHA1Sum(const char *filename);

/**
 * @brief 计算文件的 SHA-256 校验和
 *
 * @param filename 文件路径
 * @return 返回 SHA-256 校验和的字符串（需要调用者负责释放内存）
 */
char *calcFileSHA256Sum(const char *);

#endif // HASHVAL_H
