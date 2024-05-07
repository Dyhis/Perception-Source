#include <string>
#include "driver.hpp"
#include "defs.h"
#include "spoofer.h"

#define FNAMEPOOL_OFFSET  0x1198BD00

static std::string GetNameFromIndex(int key)
{
    uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
    uint16_t NameOffset = (uint16_t)key;
    uint64_t NamePoolChunk = driver.Read<uint64_t>(pid, virtualaddy + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 0x10) + (unsigned int)(2 * NameOffset);
    uint16_t nameEntry = driver.Read<uint16_t>(pid, NamePoolChunk);
    int nameLength = nameEntry >> 6;
    char buff[1024] = {};


    char* v2; // rdi
    int v4; // ebx
    __int16 result; // ax
    int v6; // edx
    int v7; // ecx
    int v8; // eax
    unsigned int v9; // ecx

    v2 = buff;
    v4 = nameLength;
    v6 = 0;
    v7 = 24;
    if (v4)
    {
        driver.Read(pid, (PVOID)(NamePoolChunk + 2), (PVOID)buff, 2 * nameLength);
        do
        {
            v8 = v6++;
            v9 = (v8 | 0xB000) + v7;
            result = v9 ^ ~*v2;
            v7 = v9 >> 2;
            *v2++ = result;
        } while (v6 < v4);
        buff[nameLength] = '\0';
        return std::string(buff);
    }
    return std::string("");
}

static std::string GetNameFromFName(int key)
{
    uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
    uint16_t NameOffset = (uint16_t)key;

    uint64_t NamePoolChunk = driver.Read<uint64_t>(pid, virtualaddy + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
    if (driver.Read<uint16_t>(pid, NamePoolChunk) < 64)
    {
        auto a1 = driver.Read<DWORD>(pid, NamePoolChunk + 2);
        return GetNameFromIndex(a1);
    }
    else
    {
        return GetNameFromIndex(key);
    }
}
