/*
 *    FileName : GetMd5File.c
 *    By : yzx
 *    MakeTime : 2024年9月5日 下午 05:42:28
 *    Copyright (C) 2023-2024 openGames
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "modules/cJSON.h"   //JSON处理模块
#include "modules/HashVal.h" //哈希值计算模块
#include "modules/CRC32.h"   //CRC32计算模块

char *concatenate(const char *str1, const char *str2, const char *str3)
{
    // 计算三个字符串长度
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len3 = strlen(str3);
    size_t total_len = len1 + len2 + len3 + 1; // +1 在末尾加空
    // 分配内存
    char *result = (char *)malloc(total_len * sizeof(char));

    if (result == NULL)
    {
        perror("验证程序异常退出 内存分配失败");
        return NULL;
    }
    // 拼接字串
    strcpy(result, str1);
    strcat(result, str2);
    strcat(result, str3);
    return result;
}
// 比较文件的哈希值
int compare_hashes(const char *file_path, cJSON *json_item)
{
    char *hash;
    bool Yes;
    const char *hash_type[] = {"MD5", "SHA1", "SHA256", "CRC32"}; // 类型
    int i;
    generate_crc32_table();
    for (i = 0; i < 4; i++)
    {
        cJSON *hash_obj = cJSON_GetObjectItemCaseSensitive(json_item, hash_type[i]);
        char *d = cJSON_Print(hash_obj);
        Yes = true; // 假设这个文件是通过的
        switch (i)
        {
        case 0:
            hash = concatenate("\"", calcFileMd5Sum(file_path), "\"");
            if (strcmp(hash, d) != 0)
            {
                Yes = false;
            }
            free(hash);
            break;
        case 1:
            hash = concatenate("\"", calcFileSHA1Sum(file_path), "\"");
            if (strcmp(hash, d) != 0)
            {
                Yes = false;
            }
            free(hash);
            break;
        case 2:
            hash = concatenate("\"", calcFileSHA256Sum(file_path), "\"");
            if (strcmp(hash, d) != 0)
            {
                Yes = false;
            }
            free(hash);
            break;
        case 3:
            hash = concatenate("\"", calcCRC32(file_path), "\"");
            if (strcmp(hash, d) != 0)
            {
                Yes = false;
            }
            free(hash);
            break;
        default:
            break;
        }
        if (Yes)
        {
            printf("文件 %s 的MD5值校验通过\n", file_path);
        }
        else
        {
            printf("文件 %s 的MD5值校验不通过\n", file_path);
        }
    }
    return 1;
}

// 遍历目录
void traverse_directory(const char *folder_path, cJSON *json_root)
{
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    struct stat statbuf;

    dir = opendir(folder_path);
    if (dir == NULL)
    {
        perror("无法打开目录");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // 忽略 . 和 .. 目录
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", folder_path, entry->d_name);

        if (stat(path, &statbuf) == -1)
        {
            perror("无法获取文件状态");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            // 递归处理子目录
            traverse_directory(path, json_root);
        }
        else if (S_ISREG(statbuf.st_mode))
        {
            // 处理文件
            cJSON *file_item = cJSON_GetObjectItemCaseSensitive(json_root, path);
            if (file_item)
            {
                if (!compare_hashes(path, file_item))
                {
                    printf("文件 %s 的哈希值不匹配\n", path);
                }
            }
        }
    }

    closedir(dir);
}

void md5gen_get(char *Md5FilePath, const char *FolderPath)
{
    FILE *ThisMD5JSON;   // 文件指针
    long Md5FileLenght;  // 文件大小
    char *Md5JsonString; // JSON文件内容

    ThisMD5JSON = fopen(Md5FilePath, "r"); // 打开JSON文件
    if (!ThisMD5JSON)
    {
        perror("验证程序异常退出 打开MD5文件失败!");
        return;
    }

    // 获取文件大小
    fseek(ThisMD5JSON, 0, SEEK_END);
    Md5FileLenght = ftell(ThisMD5JSON);
    fseek(ThisMD5JSON, 0, SEEK_SET);

    // 分配内存并读取文件内容
    Md5JsonString = (char *)malloc(Md5FileLenght + 1);
    if (!Md5JsonString)
    {
        perror("验证程序异常退出 内存分配失败!");
        fclose(ThisMD5JSON);
        return;
    }
    fread(Md5JsonString, 1, Md5FileLenght, ThisMD5JSON);
    Md5JsonString[Md5FileLenght] = '\0'; // 添加 null 终止符
    fclose(ThisMD5JSON);                 // 关闭文件

    // 解析JSON串
    cJSON *ThisMd5 = cJSON_Parse(Md5JsonString); // 解析
    if (!ThisMd5)
    {
        perror("程序异常退出 解析失败!");
        free(Md5JsonString); // 释放掉Md5JsonString
        return;
    }

    // 递归遍历文件夹
    traverse_directory(FolderPath, ThisMd5);

    // 释放JSON内存
    cJSON_Delete(ThisMd5);
    free(Md5JsonString);
}