/*
 *    FileName : HashVal.c
 *    By : yzx
 *    MakeTime : 2024年9月4日 下午 04:54:04
 *    Copyright (C) 2023-2024 openGames
 */
#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

char *calcFileSHA256Sum(const char *FilePath)
{
    FILE *file = fopen(FilePath, "rb");
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    unsigned char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        SHA256_Update(&sha256, buffer, bytes_read);
    }
    fclose(file);

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_Final(digest, &sha256);

    // 分配足够的内存用于返回的字符串
    char *sha256_string = malloc(SHA256_DIGEST_LENGTH * 2 + 1); // 64个字符 + 终止符
    if (sha256_string == NULL)
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    // 转换为十六进制字符串
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(sha256_string + i * 2, "%02x", digest[i]);
    }
    sha256_string[SHA256_DIGEST_LENGTH * 2] = '\0'; // 添加字符串终止符

    return sha256_string;
}

char *calcFileSHA1Sum(const char *FilePath)
{
    FILE *file = fopen(FilePath, "rb");
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    SHA_CTX sha1;
    SHA1_Init(&sha1);

    unsigned char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        SHA1_Update(&sha1, buffer, bytes_read);
    }
    fclose(file);

    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1_Final(digest, &sha1);

    // 分配足够的内存用于返回的字符串
    char *sha1_string = malloc(SHA_DIGEST_LENGTH * 2 + 1); // 40个字符 + 终止符
    if (sha1_string == NULL)
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    // 转换为十六进制字符串
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(sha1_string + i * 2, "%02x", digest[i]);
    }
    sha1_string[SHA_DIGEST_LENGTH * 2] = '\0'; // 添加字符串终止符

    return sha1_string;
}
char *calcFileMd5Sum(const char *FilePath)
{
    FILE *file = fopen(FilePath, "rb");
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    MD5_CTX md5;
    MD5_Init(&md5);

    unsigned char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        MD5_Update(&md5, buffer, bytes_read);
    }
    fclose(file);

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &md5);

    // 分配足够的内存用于返回的字符串
    char *md5_string = malloc(MD5_DIGEST_LENGTH * 2 + 1); // 32个字符 + 终止符
    if (md5_string == NULL)
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    // 转换为十六进制字符串
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf(md5_string + i * 2, "%02x", digest[i]);
    }
    md5_string[MD5_DIGEST_LENGTH * 2] = '\0'; // 添加字符串终止符

    return md5_string;
}