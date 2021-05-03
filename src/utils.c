/**

   Copyright 2017, 2021 Anton "Vuvk" Shcherbatykh (vuvk69@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"

#include "utils.h"
#include "defines.h"


/* math */

float fast_sqrt(float val)
{
    int32 tmp = *(int32*)&val;
    tmp -= 1 << 23;
    tmp = tmp >> 1;
    tmp += 1 << 29;
    return *(float*)&tmp;
}

/* swap anything */
void SwapPtr (void** a, void** b)
{
    void* t = *b;
    *b = *a;
    *a = t;
}

void SwapShort (int16* a, int16* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void SwapInt (int32* a, int32* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void SwapLong(int64* a, int64* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void SwapFloat (float* a, float* b)
{
    float t = *a;
    *a = *b;
    *b = t;
}

/* strings */
int8 StrEqual (const char* str1, const char* str2)
{
    // not correct!
    if (str1 == NULL || str2 == NULL)
        return -1;

    // strings equal
    if (strcmp (str1, str2) == 0)
        return true;

    // strings not equal
    return false;
}

void StrCopy(char* dst, const char* src, uint32 maxLength)
{
    uint32 len = StrLength(src);
    if (len > maxLength - 1)
        len = maxLength - 1;
    strncpy(dst, src, len*sizeof(char));
    dst[len] = '\0';
}

int32 StrLength(const char* string)
{
    if (string == NULL)
        return -1;

    int32 len = 0;
    char* chr = (char*)string;
    while ((*chr != '\0') && (len <= MAX_STRING_LENGTH))
    {
        ++chr;
        ++len;
    }

    return len;
}


bool FileExist(const char* fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f != NULL)
    {
        fclose(f);
        f = NULL;
        return true;
    }

    return false;
}

