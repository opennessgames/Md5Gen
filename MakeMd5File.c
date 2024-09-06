/*
 *    FileName : MakeMd5File.c
 *    By : yzx
 *    MakeTime : 2024年9月4日 下午 08:08:50
 *    Copyright (C) 2023-2024 openGames
 */

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "modules/HashVal.h" //计算哈希值模块
#include "modules/CRC32.h"   //计算CRC32值模块
#include "modules/cJSON.h"   //处理JSON数据模块

bool md5gen(cJSON *MD5ROOT, char *Md5FilePath, const char *FolderPath, int deep)
{
    bool integrity = false; // 默认是不完整的
    char *MD5_String, *SHA1_String, *SHA256_String, *CRC32_String;
    generate_crc32_table();
    if (MD5ROOT == NULL)
    {
        fprintf(stderr, "Failed to create JSON Object\n");
        return false;
    }
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    dir = opendir(FolderPath); // 打开文件夹
    if (dir == NULL)
    {
        perror("opendir");
        cJSON_Delete(MD5ROOT);
        return false;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        // 过滤掉以点或者两点开头的特殊目录
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        /*         else if (strcmp(entry->d_name, "MD5.json") == 0)
                {
                    printf("过滤 MD5.sjon\n");
                    continue;
                } */
        char ThisPath[1024]; // 构建完整的路径
        snprintf(ThisPath, sizeof(ThisPath), "%s/%s", FolderPath, entry->d_name);
        cJSON *MD5JSON = cJSON_CreateObject(); // 创建子对象
        if (!MD5JSON)
        {
            fprintf(stderr, "Failed to create JSON Object for %s\n", ThisPath);
            closedir(dir);
            cJSON_Delete(MD5ROOT);
            return false;
        }
        // 获取文件夹状态
        if (stat(ThisPath, &statbuf) == -1)
        {
            perror("stat");
            cJSON_Delete(MD5JSON);
            continue;
        }
        if (S_ISREG(statbuf.st_mode)) // 文件
        {
            MD5_String = calcFileMd5Sum(ThisPath);
            SHA1_String = calcFileSHA1Sum(ThisPath);
            SHA256_String = calcFileSHA256Sum(ThisPath);
            CRC32_String = calcCRC32(ThisPath);

            if (MD5_String && SHA1_String && SHA256_String && CRC32_String)
            {
                // 将哈希值添加子对象中
                cJSON_AddStringToObject(MD5JSON, "MD5", MD5_String);
                cJSON_AddStringToObject(MD5JSON, "SHA1", SHA1_String);
                cJSON_AddStringToObject(MD5JSON, "SHA256", SHA256_String);
                cJSON_AddStringToObject(MD5JSON, "CRC32", CRC32_String);

                // 将子 JSON 对象添加到主 JSON 对象中
                cJSON_AddItemToObject(MD5ROOT, ThisPath, MD5JSON);

                // 释放哈希值字符串
                free(MD5_String);
                free(SHA1_String);
                free(SHA256_String);
                free(CRC32_String);

                // 打印结果
                printf("已计算 %s 的哈希值\n ", (char *)ThisPath);
            }
            else
            {
                fprintf(stderr, "Failed to compute hash values for %s\n", ThisPath);
                cJSON_Delete(MD5JSON);
            }
        }
        else if (S_ISDIR(statbuf.st_mode)) // 文件夹
        {
            fprintf(stderr, "处理目录 %s\n", ThisPath);
            md5gen(MD5ROOT, NULL, ThisPath, deep + 1);
            cJSON_AddStringToObject(MD5ROOT, ThisPath, "DIR");
            // cJSON_Delete(MD5JSON);
        }
        else
        {
            cJSON_Delete(MD5JSON);
        }
    }
    closedir(dir);
    // 保存 JSON 到文件
    if (Md5FilePath != NULL)
    {
        struct stat path_stat;
        if (stat(Md5FilePath, &path_stat) == 0)
        {
            // 文件已存在，询问是否替换
            char response;
            printf("文件 %s 已存在，是否替换？(y/n): ", Md5FilePath);
            scanf(" %c", &response);
            if (response != 'y' && response != 'Y')
            {
                printf("取消文件保存。\n");
                cJSON_Delete(MD5ROOT);
                return false;
            }
        }
        char *jsonSTRING = cJSON_Print(MD5ROOT);
        if (jsonSTRING != NULL)
        {
            FILE *file = fopen(Md5FilePath, "w");
            if (file != NULL)
            {
                fprintf(file, "%s", jsonSTRING);
                fclose(file);
                integrity = true; // 保存成功
            }
            else
            {
                perror("fopen");
            }
            free(jsonSTRING);
        }
        else
        {
            fprintf(stderr, "Failed to print JSON\n");
        }
    }

    return integrity;
}