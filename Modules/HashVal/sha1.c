/*
 *    FileName : sha1.h
 *    By : yzx
 *    MakeTime : 2024年9月5日 下午 05:55:26
 *    Copyright (C) 2023-2024 openGames
 */
#include <stdio.h>
#include <string.h>

#define S(x, n) (((x) << n) | (x) >> (32 - n)) // SHA定义S函数为循环左移

static unsigned long h[5];
static unsigned long m[16];
static unsigned long w[80];

// 补位(余数=448)+补长度(64位)=512位M
static void sha1_pad(char *input, int len)
{
    int i;
    int n;

    for (i = 0; i < 16; i++)
    {
        m[i] = 0;
    }
    // printf("%d",len);

    for (i = 0; i < len; i++)
    {
        n = 24 - ((i & 0x03) << 3);
        m[i / 4] |= input[i] << n;
    }
    n = 24 - ((i & 0x03) << 3);
    m[i / 4] |= 0x80 << n;
    m[15] = len * 8;
}

// 由512位M生成80字W
static void sha1_prepare(void)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        w[i] = m[i];
    }
    for (i = 16; i < 80; i++)
    {
        w[i] = w[i - 16] ^ w[i - 14] ^ w[i - 8] ^ w[i - 3];
        w[i] = S(w[i], 1);
    }
}

// 由80字W计算sha1
static void sha1_calc(void)
{
    int i;
    unsigned long a, b, c, d, e, f, k;
    unsigned long temp;

    h[0] = 0x67452301;
    h[1] = 0xEFCDAB89;
    h[2] = 0x98BADCFE;
    h[3] = 0x10325476;
    h[4] = 0xC3D2E1F0;

    a = h[0];
    b = h[1];
    c = h[2];
    d = h[3];
    e = h[4];
    for (i = 0; i < 80; i++)
    {
        switch (i / 20)
        {
        case 0:
            k = 0x5A827999;
            f = (b & c) | (~b & d);
            break;
        case 1:
            k = 0x6ED9EBA1;
            f = b ^ c ^ d;
            break;
        case 2:
            k = 0x8F1BBCDC;
            f = (b & c) | (b & d) | (c & d);
            break;
        case 3:
            k = 0xCA62C1D6;
            f = b ^ c ^ d;
            break;
        }
        temp = S(a, 5) + f + e + w[i] + k;
        e = d;
        d = c;
        c = S(b, 30);
        b = a;
        a = temp;
    }
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

// SHA1算法接口
// input:待校验的数据
// len:数据长度(小于56字节)
unsigned long *sha1(char *input, int len)
{
    sha1_pad(input, len);
    sha1_prepare();
    sha1_calc();
    return h;
}