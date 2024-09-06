/*
 *    FileName : CRC32.c
 *    By : yzx
 *    MakeTime : 2024年9月4日 下午 05:04:57
 *    Copyright (C) 2023-2024 openGames
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "CRC32.h"

uint32_t crc32_table[256]; // Table for storing precomputed CRC32 values

// Function to generate the CRC32 lookup table
void generate_crc32_table()
{
    uint32_t c;
    // Iterate over all possible byte values (0 to 255)
    for (int i = 0; i < 256; i++)
    {
        c = i; // Start with the current byte value
        // Perform the CRC calculation for 8 bits
        for (int j = 8; j > 0; j--)
        {
            if (c & 1) // Check if the least significant bit is set
            {
                c = (c >> 1) ^ CRC32_POLYNOMIAL; // Apply polynomial XOR if bit is set
            }
            else
            {
                c = c >> 1; // Just shift if bit is not set
            }
        }
        crc32_table[i] = c; // Store the result in the table
    }
}

// Function to calculate the CRC32 checksum of a file and return it as a string
char *calcCRC32(const char *FilePath)
{
    FILE *file = fopen(FilePath, "rb"); // Open the file in binary mode
    if (file == NULL)                   // Check if file opening was successful
    {
        perror("验证程序异常退出 Unable to open file"); // Print error message if file cannot be opened
        return NULL;                                    // Return NULL to indicate an error
    }

    uint32_t crc = 0xFFFFFFFF; // Initial CRC value
    unsigned char byte;
    // Read the file byte by byte
    while (fread(&byte, 1, 1, file) == 1)
    {
        crc = (crc >> 8) ^ crc32_table[(crc ^ byte) & 0xFF]; // Update CRC using the lookup table
    }

    fclose(file); // Close the file

    // Allocate memory for the CRC result string
    char *crc_str = (char *)malloc(9); // 8 hex digits + 1 null terminator
    if (crc_str == NULL)
    {
        perror("验证程序异常退出 Memory allocation failed");
        return NULL; // Return NULL if memory allocation fails
    }

    // Convert the CRC value to a string and store it in crc_str
    snprintf(crc_str, 9, "%08X", crc ^ 0xFFFFFFFF);

    return crc_str; // Return the CRC value as a string
}
