/*
 *    FileName : md5gen.c
 *    By : yzx
 *    MakeTime : 2024年9月3日 下午 06:51:35
 *    Copyright (C) 2023-2024 openGames
 */

/*
    检验文件完整性

    MD5.json基本结构
    {
        "FileFloder" : "Folder",
        "2023毕业照.jpg": { //文件1
            "MD5": "",
            "SH1": "",
            "SHA256": "",
            "CRC32":""
        },
        "de05fec43942d63d618f19ac5d8e4719.jpg": { //文件2
            "MD5": "",
            "SH1": "",
            "SHA256": "",
            "CRC32":""
        }
    }

    一. 读取根目录下的MD5.json文件
        1. 读取文件对象
    2.
    实现思路 :
    一 生成MD5
        1.使用cJSON库生成JSON文件
        2.递归遍历每一个文件(夹)
        3.cJSON_CreateObject()方法创建对象(主对象和子对象)
        4.cJSON_AddStringToObject(cJSONobj, char *, char *)方法添加键值至对象
        5.保存为JSON文件
    二 读取MD5
        1.使用cJSON库读取JSON文件
        2.匹配对象名,获取这个文件的哈希值
        3.计算这个文件的哈希值
        4.将新计算的哈希值与获取源MD5.json的哈希值对比
*/

#include <stdio.h>
#include <string.h>
#include "MakeMd5File.h" //生成MD5
#include "GetMd5File.h"  //获取并验证MD5

typedef struct
{
    char *md5File;
    char *dirPath;
    char *mode;
} Options; // 选项结构体

// 帮助
void help()
{
    printf("用法说明：\n");
    printf("  -md5 [filepath]  指定保存MD5值的文件路径。\n");
    printf("  -dir [DirPath]   指定要处理的文件夹路径。\n");
    printf("  -mode [mode]     指定操作模式：\n");
    printf("                    - 参数1: 生成模式\n");
    printf("                    - 参数2: 检查模式\n");
    printf("示例：若要将指定文件夹内所有文件的MD5值生成并保存为JSON文件，可使用命令：\n");
    printf("  md5gem -md5 [filepath] -dir [DirPath] -mode 1\n");
    printf("  作者 : yzx\n");
}
// 读取并验证哈希
int main(int argc, char *argv[])
{
    Options opts = {NULL, NULL, NULL};

    for (int i = 1; i < argc; i++)
    {
        // 打印帮助
        if (strcmp(argv[0], "-help"))
        {
            help();
            return 0;
        }
        if (strcmp(argv[i], "-md5") == 0 && i + 1 < argc)
        {
            opts.md5File = argv[++i];
        }
        else if (strcmp(argv[i], "-dir") == 0 && i + 1 < argc)
        {
            opts.dirPath = argv[++i];
        }
        else if (strcmp(argv[i], "-mode") == 0 && i + 1 < argc)
        {
            opts.mode = argv[++i];
        }
    }
    (opts.md5File == NULL) ? perror("验证程序异常退出 没指定MD5文件") : printf("MD5 File: %s\n", opts.md5File);
    (opts.dirPath == NULL) ? perror("验证程序异常退出 没指定文件夹") : printf("Directory Path: %s\n", opts.dirPath);
    (opts.mode == NULL) ? perror("验证程序异常退出 没指定模式") : printf("Mode: %s\n", opts.mode);
    if (strcmp(opts.mode, "1") == 0)
    {
        cJSON *MD5ROOT = cJSON_CreateObject(); // 创建一个主对象
        if (md5gen(MD5ROOT, opts.md5File, opts.dirPath, 0))
        {
            printf("MD5 hash generation succeeded.\n");
        }
        else
        {
            printf("MD5 hash generation failed.\n");
        }
        cJSON_Delete(MD5ROOT); // 释放 MD5ROOT
    }
    else if (strcmp(opts.mode, "2") == 0)
    {
        md5gen_get(opts.md5File, opts.dirPath);
    }
    else
    {
        printf("验证程序异常退出 未知的模式");
    }
    return 0;
}