/*
 *    FileName : sha256.c
 *    By : yzx
 *    MakeTime : 2024年9月5日 下午 06:08:15
 *    Copyright (C) 2023-2024 openGames
 */
#include "sha256.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 字循环右移n位
#define wr_rotate(w, n) (((w) >> (n)) | (w) << (32 - (n)))

// 从内存中读取数据作为uint32，如src中数据为0x11,0x22,0x33,0x44...，读取后的uint32值为0x11223344
static inline void __read_uint32(uint32_t *dst, const void *src)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned char *p = (unsigned char *)dst;
    unsigned char *q = (unsigned char *)src + 3;
    for (int i = 0; i < 4; i++)
    {
        *(p++) = *(q--);
    }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    *dst = *((uint32_t *)src);
#else
#error "Unsupported target architecture endianess!"
#endif
    return;
}
// 向内存中写入uint32数据，如uint32值为0x11223344，写入后目的地址中的值为0x11,0x22,0x33,0x44
static inline void __write_uint32(void *dst, const uint32_t *src)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned char *p = (unsigned char *)dst;
    unsigned char *q = (unsigned char *)src + 3;
    for (int i = 0; i < 4; i++)
    {
        *(p++) = *(q--);
    }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    *(uint32_t *)dst = *src;
#else
#error "Unsupported target architecture endianess!"
#endif
    return;
}
// 向内存中写入uint64数据，如uint64的值为0x1122334455667788，写入后目的地址中的值为0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88
static inline void __write_uint64(void *dst, const long long unsigned int *src)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned char *p = (unsigned char *)dst;
    unsigned char *q = (unsigned char *)src + 7;
    for (int i = 0; i < 8; i++)
    {
        *(p++) = *(q--);
    }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    *(uint64_t *)dst = *src;
#else
#error "Unsupported target architecture endianess!"
#endif
    return;
}

// 为计算较大规模数据的哈希值，data_from_malloc参数必须是malloc或calloc申请的空间的指针，调用此函数后data_from_malloc中的数据会被回收
bool sha256(unsigned char *data_from_malloc, unsigned long long len, unsigned char *out)
{
    // 64个常量（前64个质数的1/2次方）
    static const uint32_t k[64] = {
        0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
        0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
        0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
        0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u, 0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
        0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
        0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
        0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
        0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u};

    // 初始的哈希结果
    uint32_t result_part_0 = 0x6a09e667u;
    uint32_t result_part_1 = 0xbb67ae85u;
    uint32_t result_part_2 = 0x3c6ef372u;
    uint32_t result_part_3 = 0xa54ff53au;
    uint32_t result_part_4 = 0x510e527fu;
    uint32_t result_part_5 = 0x9b05688cu;
    uint32_t result_part_6 = 0x1f83d9abu;
    uint32_t result_part_7 = 0x5be0cd19u;

    // 计算填充后的长度data_after_padding_len
    unsigned long long temvar = len & 0x3fllu; // temvar = len % 64llu
    unsigned long long append = (temvar < 56) ? (56 - temvar) : (56 + 64 - temvar);
    unsigned long long data_after_padding_len = len + append + 8;

    // 填充数据到data_after_padding，防止栈空间溢出，使用realloc
    unsigned char *data_after_padding;
    if ((data_after_padding = realloc(data_from_malloc, data_after_padding_len)) == NULL)
    {
        fputs("Unable to obtain sufficient memory!\n", stderr);
        return false;
    }
    unsigned char *p = data_after_padding + len;
    unsigned char *zero_padding_end = p + append;
    *(p++) = 0x80u;
    while (p < zero_padding_end)
    {
        *(p++) = 0x00u;
    }
    unsigned long long bits_len = len << 3; // bits_len = len * 8
    __write_uint64(p, &bits_len);

    uint32_t w[64]; // 每个区块构造的扩展（64个字）
    unsigned char *current_data = data_after_padding;
    unsigned char *data_end = data_after_padding + data_after_padding_len;
    while (current_data < data_end)
    {
        // 构造64个字
        uint32_t *current_w = w;
        uint32_t *front16_w = w + 16;
        uint32_t *end_w = w + 64;
        // 构造前16个字
        while (current_w < front16_w)
        {
            __read_uint32(current_w++, current_data);
            current_data += 4;
        }
        // 构造其余的字
        while (current_w < end_w)
        {
            uint32_t s0 = wr_rotate(*(current_w - 15), 7) ^ wr_rotate(*(current_w - 15), 18) ^ (*(current_w - 15) >> 3);
            uint32_t s1 = wr_rotate(*(current_w - 2), 17) ^ wr_rotate(*(current_w - 2), 19) ^ (*(current_w - 2) >> 10);
            *current_w = *(current_w - 16) + s0 + *(current_w - 7) + s1;
            ++current_w;
        }

        // 根据构造的64个字、当前的哈希结果和64个常量进行64轮运算，运算结果放入a~h中
        uint32_t a = result_part_0;
        uint32_t b = result_part_1;
        uint32_t c = result_part_2;
        uint32_t d = result_part_3;
        uint32_t e = result_part_4;
        uint32_t f = result_part_5;
        uint32_t g = result_part_6;
        uint32_t h = result_part_7;
        current_w = w;
        const uint32_t *current_k = k;
        while (current_w < end_w)
        {
            uint32_t s_1 = wr_rotate(e, 6) ^ wr_rotate(e, 11) ^ wr_rotate(e, 25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp = h + s_1 + ch + *(current_k++) + *(current_w++);
            uint32_t s_0 = wr_rotate(a, 2) ^ wr_rotate(a, 13) ^ wr_rotate(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            h = g;
            g = f;
            f = e;
            e = d + temp;
            d = c;
            c = b;
            b = a;
            a = temp + maj + s_0;
        }

        // 更新当前的哈希结果
        result_part_0 += a;
        result_part_1 += b;
        result_part_2 += c;
        result_part_3 += d;
        result_part_4 += e;
        result_part_5 += f;
        result_part_6 += g;
        result_part_7 += h;
    }
    // 释放空间
    free(data_after_padding);

    // 导出哈希结果
    __write_uint32(out, &result_part_0);
    out += 4;
    __write_uint32(out, &result_part_1);
    out += 4;
    __write_uint32(out, &result_part_2);
    out += 4;
    __write_uint32(out, &result_part_3);
    out += 4;
    __write_uint32(out, &result_part_4);
    out += 4;
    __write_uint32(out, &result_part_5);
    out += 4;
    __write_uint32(out, &result_part_6);
    out += 4;
    __write_uint32(out, &result_part_7);
    return true;
}