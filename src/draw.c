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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
//#include <time.h>

#include "libfixmath/fixmath.h"

#include "draw.h"
#include "utils.h"
#include "render_private.h"

// граница рисования горизонтальной линии
typedef struct
{
    int32 x0, x1;
} SBorderBit;

// граница рисования горизонтальной линии с градацией цвета
typedef struct
{
    int32 x0, x1;
    uint32 color0, color1;
} SBorderBitColor;

// граница рисования горизонтальной линии с градацией текстурной координаты
typedef struct
{
    int32 x0, x1;
    TrVector2f uv0, uv1;
} SBorderBitUV;

// аргументы для многопоточного рендеринга
typedef struct
{
    void* pixels;
    void* borders;
    uint32 color;
    STexture* texture;
    int minX, minY;
    int fromY;
    int toY;
} SThreadArgs;

static int renderTask = 0;  // количество задач для рендеринга
static int renderDone = 0;  // количество завершенных задач


void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color, void* pixels)
{
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
    {
        tr_swap (&x0, &y0);
        tr_swap (&x1, &y1);
        steep = true;
    }

    if (x0 > x1)
    {
        tr_swap (&x0, &x1);
        tr_swap (&y0, &y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = abs(dy) << 1;
    int error2 = 0;
    int y = y0;
    for (register int x = x0; x <= x1; ++x)
    {
        if (steep)
        {
            PutPixel(y, x, color, pixels);
        }
        else
        {
            PutPixel(x, y, color, pixels);
        }
        error2 += derror2;

        if (error2 > dx)
        {
            y += ((y1 > y0) ? 1 : -1);
            error2 -= dx << 1;
        }
    }

    /*
    int diffx = x1 - x0;
    int diffy = y1 - y0;

    int len = MAX(abs(diffx), abs(diffy)) + 1;
    float lenInverse = 1.0 / len;
    float dx = diffx * lenInverse;
    float dy = diffy * lenInverse;

    float x = x0;
    float y = y0;

    while (len > 0)
    {
        PutPixel(x, y, color, pixels);
        x += dx;
        y += dy;

        --len;
    }
    */
}

void DrawLineBicolor (int32 x0, int32 y0, int32 x1, int32 y1, uint32 color0, uint32 color1, void* pixels)
{
    int diffx = x1 - x0;
    int diffy = y1 - y0;

    int len = MAX(abs(diffx), abs(diffy)) + 1;
    float lenInverse = 1.0 / len;
    float dx = diffx * lenInverse;
    float dy = diffy * lenInverse;

    float x = x0;
    float y = y0;

    double r = RedFromRGB(color0);
    double g = GreenFromRGB(color0);
    double b = BlueFromRGB(color0);

    int16 dR = RedFromRGB(color1)   - r;
    int16 dG = GreenFromRGB(color1) - g;
    int16 dB = BlueFromRGB(color1)  - b;

    // шаг цветовой интерполяции
    double rDiff = dR * lenInverse;
    double gDiff = dG * lenInverse;
    double bDiff = dB * lenInverse;

    uint32 color = color0;

    while (len > 0)
    {
        color = RGB_TO_COLOR((uint8)r, (uint8)g, (uint8)b);

        PutPixel(x, y, color, pixels);
        x += dx;
        y += dy;

        r += rDiff;
        g += gDiff;
        b += bDiff;

        --len;
    }
}


static void DrawHorLineColor(int32 x0, int32 x1, int32 y, uint32 color, void* pixels)
{
    if (y < 0 || y > trHeight - 1)
        return;

    LIMIT(x0, 0, trWidth - 1);
    LIMIT(x1, 0, trWidth - 1);

    uint8* p = (uint8*)pixels + y * trPitch + x0 * trBpp;
    for (register int x = x0; x <= x1; ++x, p += trBpp)
        *(uint32*)p = color;
}

static void DrawHorLineBicolor(int32 x0, int32 x1, int32 y, uint32 color0, uint32 color1, void* pixels)
{
    // что быстрее???
    // ЭТО
    if (y < 0 || y > trHeight - 1)
        return;

    LIMIT(x0, 0, trWidth - 1);
    LIMIT(x1, 0, trWidth - 1);

    uint8* p = ((uint8*)pixels) + y * trPitch + x0 * trBpp;

    //int len = x1 - x0;
    //double lenInverse = 1.0 / len;
    double lenInverse = 1.0 / (x1 - x0);

    register double r = RedFromRGB(color0);
    register double g = GreenFromRGB(color0);
    register double b = BlueFromRGB(color0);

    register int _r, _g, _b;

    /*
    int _prevR = 0,
        _prevG = 0,
        _prevB = 0;
    */

    register uint32 color;

    int dR = RedFromRGB(color1)   - r;
    int dG = GreenFromRGB(color1) - g;
    int dB = BlueFromRGB(color1)  - b;

    // шаг цветовой интерполяции
    register double rDiff = dR * lenInverse;
    register double gDiff = dG * lenInverse;
    register double bDiff = dB * lenInverse;

    // проходимся до предпоследнего пикселя, потому что при добавлении к компоненту (r += diffR)
    // цвет может выехать за последний цвет
    for (register int x = x0; x < x1; ++x, p += trBpp)
    {
        _r = r;
        _g = g;
        _b = b;

        /*
        if (_prevR != _r || _prevG != _g || _prevB != _b)
        {
            color = RGB_TO_COLOR(_r, _g, _b);
            _prevR = _r;
            _prevG = _g;
            _prevB = _b;
        }
        */

        color = RGB_TO_COLOR(_r, _g, _b);
        *(int*)p = color;

        r += rDiff;
        g += gDiff;
        b += bDiff;
    }

    // последний пиксель
    *(int*)p = color1;
}

static void DrawHorLineTextured(int32 x0, int32 x1, int32 y, TrVector2f uv0, TrVector2f uv1, STexture* texture, void* pixels)
{
    if (y < 0 || y > trHeight - 1)
        return;

    LIMIT(x0, 0, trWidth - 1);
    LIMIT(x1, 0, trWidth - 1);

    uint8* p = ((uint8*)pixels) + y * trPitch + x0 * trBpp;
    int len = x1 - x0;

    TrVector2f uv = uv0;
    float lenInverse = 1.0f / len;
    TrVector2f uvStep = tr_vec2f_new (
        (uv1.x - uv0.x) * lenInverse,
        (uv1.y - uv0.y) * lenInverse
    );

    register int texX = 0, texY = 0;
    register int u, v;
    register int prevU = 0, prevV = 0;
    register int texW = texture->width,
                 texH = texture->height;
    register int texPitch = texture->pitch;
    uint8* texPixels = texture->pixels;

    // если строка/колонка текстуры не изменилась, то зачем пересчитывать?
    uint8* texCol = texPixels;
    register int texRow = 0;

    // проход
    for (register int x = x0; x <= x1; ++x, p += trBpp)
    {
        u = uv.x * texW;
        v = uv.y * texH;

        if (prevU != u)
        {
            texX = (u % texW);
            texRow = texX * trBpp;
            prevU = u;
        }

        if (prevV != v)
        {
            texY = (v % texH);
            texCol = texPixels + texY * texPitch;
            prevV = v;
        }

        *(uint32*)p = *(uint32*)(texCol + texRow);

        uv.x += uvStep.x;
        uv.y += uvStep.y;
    }
}

static void DrawBorderBit(int32 x0, int32 y0, int32 x1, int32 y1, uint32 h, SBorderBit* borders)
{
    if (y0 > y1)
    {
        tr_swap (&x0, &x1);
        tr_swap (&y0, &y1);
    }

    int len = y1 - y0 + 1;

    register int _x;
    register double x = x0;
    register double dx = (double)(x1 - x0) / len;

    register SBorderBit* p = borders + y0;
    while (len > 0)
    {
        _x = (int)x;

        if (p->x0 < 0)
        {
            p->x0 = _x;
            p->x1 = _x;
        }
        else
        {
            if (p->x0 > _x)
                p->x0 = _x;

            if (p->x1 < _x)
                p->x1 = _x;
        }

        x += dx;
        ++p;

        --len;
    }
}

static void DrawBorderBitColor(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color0, uint32 color1, uint32 h, SBorderBitColor* borders)
{
    if (y0 > y1)
    {
        tr_swap (&x0, &x1);
        tr_swap (&y0, &y1);
        tr_swap ((int32*)&color0, (int32*)&color1);
    }

    int len = y1 - y0 + 1;
    double lenInverse = 1.0 / len;

    register int _x;
    register double x = x0;
    register double dx = (double)(x1 - x0) * lenInverse;

    register  double r = RedFromRGB(color0);
    register double g = GreenFromRGB(color0);
    register double b = BlueFromRGB(color0);

    int16 dR = RedFromRGB(color1)   - r;
    int16 dG = GreenFromRGB(color1) - g;
    int16 dB = BlueFromRGB(color1)  - b;

    // шаг цветовой интерполяции
    register double rDiff = dR * lenInverse;
    register double gDiff = dG * lenInverse;
    register double bDiff = dB * lenInverse;

    register SBorderBitColor* p = borders + y0;
    while (len > 0)
    {
        _x = (int)x;

        if (p->x0 < 0)
        {
            p->x0 = _x;
            p->x1 = _x;
            p->color0 = RGB_TO_COLOR((uint8)r, (uint8)g, (uint8)b);
            p->color1 = RGB_TO_COLOR((uint8)r, (uint8)g, (uint8)b);
        }
        else
        {
            if (p->x0 > _x)
            {
                p->x0 = _x;
                p->color0 = RGB_TO_COLOR((uint8)r, (uint8)g, (uint8)b);
            }

            if (p->x1 < _x)
            {
                p->x1 = _x;
                p->color1 = RGB_TO_COLOR((uint8)r, (uint8)g, (uint8)b);
            }
        }

        x += dx;

        r += rDiff;
        g += gDiff;
        b += bDiff;

        ++p;

        --len;
    }
}

static void DrawBorderBitUV(int32 x0, int32 y0, int32 x1, int32 y1, TrVector2f uv0, TrVector2f uv1, SBorderBitUV* borders)
{
    if (y0 > y1)
    {
        tr_swap (&x0, &x1);
        tr_swap (&y0, &y1);
        tr_swap (&uv0.x, &uv1.x);
        tr_swap (&uv0.y, &uv1.y);
    }

    int len = y1 - y0 + 1;
    float lenInverse = 1.0f / len;

    TrVector2f uv = uv0;
    TrVector2f uvStep = tr_vec2f_new (
        (uv1.x - uv0.x) * lenInverse,
        (uv1.y - uv0.y) * lenInverse
    );

    register int _x;
    register double x = x0;
    register double dx = (double)(x1 - x0) * lenInverse;

    SBorderBitUV* p = borders + y0;
    for (register int i = 0; i < len; ++i, ++p)
    {
        _x = (int)x;

        if (p->x0 < 0)
        {
            p->x0  = _x;
            p->x1  = _x;
            p->uv0 = uv;
            p->uv1 = uv;
        }
        else
        {
            if (p->x0 > _x)
            {
                p->x0  = _x;
                p->uv0 = uv;
            }

            if (p->x1 < _x)
            {
                p->x1  = _x;
                p->uv1 = uv;
            }
        }

        x += dx;

        uv.x += uvStep.x;
        uv.y += uvStep.y;
    }
}

// поток для рисования одноцветных горизонтальных линий
static void thrd_DrawBorderColor(void* arguments)
{
    SThreadArgs* args = arguments;

    void* pixels = args->pixels;
    uint32 color = args->color;
    int minX = args->minX;
    int minY = args->minY;
    int toY  = args->toY;

    SBorderBit* p = (SBorderBit*)args->borders + args->fromY;

    for (register int i = args->fromY; i < toY; ++i, ++p)
        DrawHorLineColor(p->x0 + minX, p->x1 + minX, i + minY, color, pixels);

    //mtx_lock(&renderLock);
    ++renderDone;
    //mtx_unlock(&renderLock);
}

// рисует одноцветный полигон по инфе из borders, разделяя рендеринг на потоки
static void DrawPolyColor(SBorderBit* borders, void* pixels, uint32 color, int minX, int minY, int bufHeight)
{
    SThreadArgs args0 = { pixels, borders, color, NULL, minX, minY, 0, bufHeight };
    SThreadArgs args1 = args0,
                args2 = args0,
                args3 = args0,
                args4 = args0,
                args5 = args0,
                args6 = args0,
                args7 = args0;
    renderDone = 0;
    /*if (bufHeight > 32)
    {
        int bufHeightPart = bufHeight >> 3;

        args0.fromY = 0;
        args0.toY   = bufHeightPart;                    // 1/8

        args1.fromY = bufHeightPart;                    // 1/8
        args1.toY   = bufHeightPart << 1;               // 2/8

        args2.fromY = bufHeightPart << 1;               // 2/8
        args2.toY   = (bufHeight >> 1) - bufHeightPart; // 3/8

        args3.fromY = (bufHeight >> 1) - bufHeightPart; // 3/8
        args3.toY   = bufHeight >> 1;                   // 4/8

        args4.fromY = bufHeight >> 1;                   // 4/8
        args4.toY   = (bufHeight >> 1) + bufHeightPart; // 5/8

        args5.fromY = (bufHeight >> 1) + bufHeightPart; // 5/8
        args5.toY   = (bufHeight >> 1) +
                      (bufHeightPart << 1);             // 6/8

        args6.fromY = (bufHeight >> 1) +
                      (bufHeightPart << 1);             // 6/8
        args6.toY   = bufHeight - bufHeightPart;        // 7/8

        args7.fromY = bufHeight - bufHeightPart;        // 7/8
        args7.toY   = bufHeight;                        // 8/8

        renderTask = 8;
        threadpool_add(threadPool, thrd_DrawBorderColor, &args0, 0);
        threadpool_add(threadPool, thrd_DrawBorderColor, &args1, 0);
        threadpool_add(threadPool, thrd_DrawBorderColor, &args2, 0);
        threadpool_add(threadPool, thrd_DrawBorderColor, &args3, 0);
        threadpool_add(threadPool, thrd_DrawBorderColor, &args4, 0);
        threadpool_add(threadPool, thrd_DrawBorderColor, &args5, 0);
        threadpool_add(threadPool, thrd_DrawBorderColor, &args6, 0);
        threadpool_add(threadPool, thrd_DrawBorderColor, &args7, 0);
    }
    else*/    // маленькие изображения рисовать в один поток
    {
        renderTask = 1;
        //threadpool_add(threadPool, thrd_DrawBorderColor, &args0, 0);
        thrd_DrawBorderColor (&args0);
    }

    /*struct timespec tw = {0, 1};
    while(renderTask > renderDone)
    {
        nanosleep(&tw, NULL);
        //usleep(1);
    }*/
}

// поток для рисования градиентных горизонтальных линий
static void thrd_DrawBorderGradient(void* arguments)
{
    SThreadArgs* args = arguments;

    void* pixels = args->pixels;
    int minX = args->minX;
    int minY = args->minY;
    int toY  = args->toY;

    SBorderBitColor* p = (SBorderBitColor*)args->borders + args->fromY;

    for (register int i = args->fromY; i < toY; ++i, ++p)
        DrawHorLineBicolor(p->x0 + minX, p->x1 + minX, i + minY, p->color0, p->color1, pixels);

    //mtx_lock(&renderLock);
    ++renderDone;
    //mtx_unlock(&renderLock);
}

// рисует затекстурированный полигон по инфе из borders, разделяя рендеринг на потоки
static void DrawPolyGradient(SBorderBitColor* borders, void* pixels, int minX, int minY, int bufHeight)
{
    SThreadArgs args0 = { pixels, borders, 0, NULL, minX, minY, 0, bufHeight };
    SThreadArgs args1 = args0,
                args2 = args0,
                args3 = args0,
                args4 = args0,
                args5 = args0,
                args6 = args0,
                args7 = args0;
    renderDone = 0;
    /*if (bufHeight > 32)
    {
        int bufHeightPart = bufHeight >> 3;

        args0.fromY = 0;
        args0.toY   = bufHeightPart;                    // 1/8

        args1.fromY = bufHeightPart;                    // 1/8
        args1.toY   = bufHeightPart << 1;               // 2/8

        args2.fromY = bufHeightPart << 1;               // 2/8
        args2.toY   = (bufHeight >> 1) - bufHeightPart; // 3/8

        args3.fromY = (bufHeight >> 1) - bufHeightPart; // 3/8
        args3.toY   = bufHeight >> 1;                   // 4/8

        args4.fromY = bufHeight >> 1;                   // 4/8
        args4.toY   = (bufHeight >> 1) + bufHeightPart; // 5/8

        args5.fromY = (bufHeight >> 1) + bufHeightPart; // 5/8
        args5.toY   = (bufHeight >> 1) +
                      (bufHeightPart << 1);             // 6/8

        args6.fromY = (bufHeight >> 1) +
                      (bufHeightPart << 1);             // 6/8
        args6.toY   = bufHeight - bufHeightPart;        // 7/8

        args7.fromY = bufHeight - bufHeightPart;        // 7/8
        args7.toY   = bufHeight;                        // 8/8

        renderTask = 8;
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args0, 0);
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args1, 0);
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args2, 0);
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args3, 0);
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args4, 0);
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args5, 0);
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args6, 0);
        threadpool_add(threadPool, thrd_DrawBorderGradient, &args7, 0);
    }
    else */   // маленькие изображения рисовать в один поток
    {
        renderTask = 1;
        //threadpool_add(threadPool, thrd_DrawBorderGradient, &args0, 0);
        thrd_DrawBorderGradient (&args0);
    }

    /*struct timespec tw = {0, 1};
    while(renderTask > renderDone)
    {
        nanosleep(&tw, NULL);
        //usleep(1);
    }*/
}

// поток для рисования текстурированных горизонтальных линий
static void thrd_DrawBorderTextured (void* arguments)
{
    SThreadArgs* args = arguments;

    void* pixels = args->pixels;
    STexture* texture = args->texture;
    int minX = args->minX;
    int minY = args->minY;
    int toY  = args->toY;

    SBorderBitUV* p = (SBorderBitUV*)args->borders + args->fromY;

    for (register int i = args->fromY; i < toY; ++i, ++p)
        DrawHorLineTextured(p->x0 + minX, p->x1 + minX, i + minY, p->uv0, p->uv1, texture, pixels);

    //mtx_lock(&renderLock);
    ++renderDone;
    //mtx_unlock(&renderLock);
}

// рисует затекстурированный полигон по инфе из borders, разделяя рендеринг на потоки
static void DrawPolyTextured(SBorderBitUV* borders, STexture* texture, void* pixels, int minX, int minY, int bufHeight)
{
    SThreadArgs args0 = { pixels, borders, 0, texture, minX, minY, 0, bufHeight };
    SThreadArgs args1 = args0,
                args2 = args0,
                args3 = args0,
                args4 = args0,
                args5 = args0,
                args6 = args0,
                args7 = args0;
    renderDone = 0;
    /*if (bufHeight > 32)
    {
        int bufHeightPart = bufHeight >> 3;

        args0.fromY = 0;
        args0.toY   = bufHeightPart;                    // 1/8

        args1.fromY = bufHeightPart;                    // 1/8
        args1.toY   = bufHeightPart << 1;               // 2/8

        args2.fromY = bufHeightPart << 1;               // 2/8
        args2.toY   = (bufHeight >> 1) - bufHeightPart; // 3/8

        args3.fromY = (bufHeight >> 1) - bufHeightPart; // 3/8
        args3.toY   = bufHeight >> 1;                   // 4/8

        args4.fromY = bufHeight >> 1;                   // 4/8
        args4.toY   = (bufHeight >> 1) + bufHeightPart; // 5/8

        args5.fromY = (bufHeight >> 1) + bufHeightPart; // 5/8
        args5.toY   = (bufHeight >> 1) +
                      (bufHeightPart << 1);             // 6/8

        args6.fromY = (bufHeight >> 1) +
                      (bufHeightPart << 1);             // 6/8
        args6.toY   = bufHeight - bufHeightPart;        // 7/8

        args7.fromY = bufHeight - bufHeightPart;        // 7/8
        args7.toY   = bufHeight;                        // 8/8

        renderTask = 8;
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args0, 0);
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args1, 0);
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args2, 0);
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args3, 0);
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args4, 0);
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args5, 0);
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args6, 0);
        threadpool_add(threadPool, thrd_DrawBorderTextured, &args7, 0);
    }
    else  */  // маленькие изображения рисовать в один поток
    {
        renderTask = 1;
        //threadpool_add(threadPool, thrd_DrawBorderTextured, &args0, 0);
        thrd_DrawBorderTextured (&args0);
    }

    /*struct timespec tw = {0, 1};
    while(renderTask > renderDone)
    {
        nanosleep(&tw, NULL);
        //usleep(1);
    }*/
}


void DrawRect(int32 x, int32 y, uint32 w, uint32 h, uint32 color, void* pixels)
{
    if ((x + w < 0) ||
        (y + h < 0) ||

        (x > trWidth ) ||
        (y > trHeight))
    {
        return;
    }

    int fromX = x;
    int fromY = y;
    int toX   = x + w;
    int toY   = y + h;

    LIMIT(fromX, 0, trWidth - 1);
    LIMIT(toX,   0, trWidth - 1);
    LIMIT(fromY, 0, trHeight - 1);
    LIMIT(toY,   0, trHeight - 1);

    for (register int i = fromY; i <= toY; ++i)
        DrawHorLineColor(fromX, toX, i, color, pixels);
}

void DrawTriangleColor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, void* pixels)
{
    int minX, minY;
    int maxX, maxY;

    minX = MIN(x0, x1);
    minX = MIN(minX, x2);
    minY = MIN(y0, y1);
    minY = MIN(minY, y2);

    maxX = MAX(x0, x1);
    maxX = MAX(maxX, x2);
    maxY = MAX(y0, y1);
    maxY = MAX(maxY, y2);

    int bufHeight = maxY - minY + 1;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    SBorderBit* borders = _alloca(bufHeight * sizeof(SBorderBit));
#else
    SBorderBit* borders = alloca(bufHeight * sizeof(SBorderBit));
#endif
    SBorderBit* p = borders;

    // обнуляем границы
    for (register int i = 0; i < bufHeight; ++i, ++p)
        p->x0 = p->x1 = -99999;

    //p = borders;

    DrawBorderBit(x0 - minX, y0 - minY, x1 - minX, y1 - minY, bufHeight, borders);
    DrawBorderBit(x1 - minX, y1 - minY, x2 - minX, y2 - minY, bufHeight, borders);
    DrawBorderBit(x0 - minX, y0 - minY, x2 - minX, y2 - minY, bufHeight, borders);

    DrawPolyColor(borders, pixels, color, minX, minY, bufHeight);

    /*
    int startX, endX;
    for (register int j = 0; j < bufHeight; ++j)
    {
        // ищем точку начала линии и конца
        startX = p->x0;
        endX   = p->x1;

        if (startX != -1)
        {
            // если концы совпали, то просто поставить точку
            if (startX == endX)
                PutPixel(startX + minX, j + minY, color, pixels);
            // а иначе нарисовать линию
            else
                DrawHorLineColor(startX + minX, endX + minX, j + minY, color, pixels);
        }

        ++p;
    }

    //free(borders);
    */
}

void DrawTriangleTricolor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color0, uint32 color1, uint32 color2, void* pixels)
{
    int minX, minY;
    int maxX, maxY;

    minX = MIN(x0, x1);
    minX = MIN(minX, x2);
    minY = MIN(y0, y1);
    minY = MIN(minY, y2);

    maxX = MAX(x0, x1);
    maxX = MAX(maxX, x2);
    maxY = MAX(y0, y1);
    maxY = MAX(maxY, y2);

    int bufHeight = maxY - minY + 1;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    SBorderBitColor* borders = _alloca(bufHeight * sizeof(SBorderBitColor));
#else
    SBorderBitColor* borders = alloca(bufHeight * sizeof(SBorderBitColor));
#endif
    SBorderBitColor* p = borders;

    // обнуляем границы
    for (register int i = 0; i < bufHeight; ++i, ++p)
        p->x0 = p->x1 = -99999;

    //p = borders;

    DrawBorderBitColor(x0 - minX, y0 - minY, x1 - minX, y1 - minY, color0, color1, bufHeight, borders);
    DrawBorderBitColor(x1 - minX, y1 - minY, x2 - minX, y2 - minY, color1, color2, bufHeight, borders);
    DrawBorderBitColor(x0 - minX, y0 - minY, x2 - minX, y2 - minY, color0, color2, bufHeight, borders);

    DrawPolyGradient(borders, pixels, minX, minY, bufHeight);
}

void DrawTriangleTextured(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2,
                          float u0, float v0, float u1, float v1, float u2, float v2,
                          STexture* texture, void* pixels)
{
    int minX, minY;
    int maxX, maxY;

    minX = MIN(x0, x1);
    minX = MIN(minX, x2);
    minY = MIN(y0, y1);
    minY = MIN(minY, y2);

    maxX = MAX(x0, x1);
    maxX = MAX(maxX, x2);
    maxY = MAX(y0, y1);
    maxY = MAX(maxY, y2);

    int bufHeight = maxY - minY + 1;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    SBorderBitUV* borders = _alloca(bufHeight * sizeof(SBorderBitUV));
#else
    SBorderBitUV* borders = alloca(bufHeight * sizeof(SBorderBitUV));
#endif
    SBorderBitUV* p = borders;

    // обнуляем границы
    for (register int i = 0; i < bufHeight; ++i, ++p)
        p->x0 = p->x1 = -99999;

    //p = borders;

    TrVector2f uv0 = tr_vec2f_new (u0, v0);
    TrVector2f uv1 = tr_vec2f_new (u1, v1);
    TrVector2f uv2 = tr_vec2f_new (u2, v2);

    DrawBorderBitUV(x0 - minX, y0 - minY, x1 - minX, y1 - minY, uv0, uv1, borders);
    DrawBorderBitUV(x1 - minX, y1 - minY, x2 - minX, y2 - minY, uv1, uv2, borders);
    DrawBorderBitUV(x0 - minX, y0 - minY, x2 - minX, y2 - minY, uv0, uv2, borders);

    DrawPolyTextured(borders, texture, pixels, minX, minY, bufHeight);
}


void DrawQuadColor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, uint32 color, void* pixels)
{
    int minX, minY;
    int maxX, maxY;

    minX = MIN(x0, x1);
    minX = MIN(minX, x2);
    minX = MIN(minX, x3);
    minY = MIN(y0, y1);
    minY = MIN(minY, y2);
    minY = MIN(minY, y3);

    maxX = MAX(x0, x1);
    maxX = MAX(maxX, x2);
    maxX = MAX(maxX, x3);
    maxY = MAX(y0, y1);
    maxY = MAX(maxY, y2);
    maxY = MAX(maxY, y3);

    //int bufWidth  = maxX - minX + 1;
    int bufHeight = maxY - minY + 1;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    SBorderBit* borders = _alloca(bufHeight * sizeof(SBorderBit));
#else
    SBorderBit* borders = alloca(bufHeight * sizeof(SBorderBit));
#endif
    SBorderBit* p = borders;

    // обнуляем границы
    for (register int i = 0; i < bufHeight; ++i, ++p)
        p->x0 = p->x1 = -99999;

    //p = borders;

    DrawBorderBit(x0 - minX, y0 - minY, x1 - minX, y1 - minY, bufHeight, borders);
    DrawBorderBit(x1 - minX, y1 - minY, x2 - minX, y2 - minY, bufHeight, borders);
    DrawBorderBit(x2 - minX, y2 - minY, x3 - minX, y3 - minY, bufHeight, borders);
    DrawBorderBit(x0 - minX, y0 - minY, x3 - minX, y3 - minY, bufHeight, borders);

    DrawPolyColor(borders, pixels, color, minX, minY, bufHeight);
    /*
    int startX, endX;
    for (register int j = 0; j < bufHeight; ++j)
    {
        // ищем точку начала линии и конца
        startX = p->x0;
        endX   = p->x1;

        if (startX != -1)
        {
            // если концы совпали, то просто поставить точку
            if (startX == endX)
                PutPixel(startX + minX, j + minY, color, pixels);
            // а иначе нарисовать линию
            else
                DrawHorLineColor(startX + minX, endX + minX, j + minY, color, pixels);
        }

        ++p;
    }

    //free(borders);
    */
}

void DrawQuadFourcolor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, uint32 color0, uint32 color1, uint32 color2, uint32 color3, void* pixels)
{
    int minX, minY;
    int maxX, maxY;

    minX = MIN(x0, x1);
    minX = MIN(minX, x2);
    minX = MIN(minX, x3);
    minY = MIN(y0, y1);
    minY = MIN(minY, y2);
    minY = MIN(minY, y3);

    maxX = MAX(x0, x1);
    maxX = MAX(maxX, x2);
    maxX = MAX(maxX, x3);
    maxY = MAX(y0, y1);
    maxY = MAX(maxY, y2);
    maxY = MAX(maxY, y3);

    int bufHeight = maxY - minY + 1;

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    SBorderBitColor* borders = _alloca(bufHeight * sizeof(SBorderBitColor));
#else
    SBorderBitColor* borders = alloca(bufHeight * sizeof(SBorderBitColor));
#endif
    SBorderBitColor* p = borders;

    // обнуляем границы
    for (register int i = 0; i < bufHeight; ++i, ++p)
        p->x0 = p->x1 = -99999;

    //p = borders;

    DrawBorderBitColor(x0 - minX, y0 - minY, x1 - minX, y1 - minY, color0, color1, bufHeight, borders);
    DrawBorderBitColor(x1 - minX, y1 - minY, x2 - minX, y2 - minY, color1, color2, bufHeight, borders);
    DrawBorderBitColor(x2 - minX, y2 - minY, x3 - minX, y3 - minY, color2, color3, bufHeight, borders);
    DrawBorderBitColor(x0 - minX, y0 - minY, x3 - minX, y3 - minY, color0, color3, bufHeight, borders);

    DrawPolyGradient(borders, pixels, minX, minY, bufHeight);
}

void DrawQuadTextured(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3,
                      float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                      STexture* texture, void* pixels)
{
    int minX, minY;
    int maxX, maxY;

    minX = MIN(x0, x1);
    minX = MIN(minX, x2);
    minX = MIN(minX, x3);
    minY = MIN(y0, y1);
    minY = MIN(minY, y2);
    minY = MIN(minY, y3);

    maxX = MAX(x0, x1);
    maxX = MAX(maxX, x2);
    maxX = MAX(maxX, x3);
    maxY = MAX(y0, y1);
    maxY = MAX(maxY, y2);
    maxY = MAX(maxY, y3);


    int bufHeight = maxY - minY + 1;

    //SBorderBitUV* borders = malloc(bufHeight * sizeof(SBorderBitUV));
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    SBorderBitUV* borders = _alloca(bufHeight * sizeof(SBorderBitUV));
#else
    SBorderBitUV* borders = alloca(bufHeight * sizeof(SBorderBitUV));
#endif
    SBorderBitUV* p = borders;

    // обнуляем границы
    for (register int i = 0; i < bufHeight; ++i, ++p)
        p->x0 = p->x1 = -99999;

    TrVector2f uv0 = tr_vec2f_new (u0, v0);
    TrVector2f uv1 = tr_vec2f_new (u1, v1);
    TrVector2f uv2 = tr_vec2f_new (u2, v2);
    TrVector2f uv3 = tr_vec2f_new (u3, v3);

    DrawBorderBitUV(x0 - minX, y0 - minY, x1 - minX, y1 - minY, uv0, uv1, borders);
    DrawBorderBitUV(x1 - minX, y1 - minY, x2 - minX, y2 - minY, uv1, uv2, borders);
    DrawBorderBitUV(x2 - minX, y2 - minY, x3 - minX, y3 - minY, uv2, uv3, borders);
    DrawBorderBitUV(x0 - minX, y0 - minY, x3 - minX, y3 - minY, uv0, uv3, borders);

    DrawPolyTextured(borders, texture, pixels, minX, minY, bufHeight);
}


void ClearPixels(uint32 color, int w, int h, void* pixels)
{
    int len = w * h;
    uint8* p = pixels;
    for (register int i = 0; i < len; ++i, p += trBpp)
        *(uint32*)p = color;
}

void PutPixel (int32 x, int32 y, uint32 color, void* pixels)
{
	if (x < 0 || y < 0 || x >= trWidth || y >= trHeight)
        return;

    *(uint32*)((uint8*)pixels + y * trPitch + x * trBpp) = color;
}

uint32 GetPixel_(uint32 x, uint32 y, void* pixels)
{
#ifdef OLD_CODE
	int bpp = frameBuffer->format->BytesPerPixel;
	uint8* p = (uint8*)frameBuffer->pixels + y * frameBuffer->pitch + x * bpp;
	switch(bpp)
	{
		case 1:
			return* p;
			break;
		case 2:
			return *(Uint16*)p;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;
		case 4:
			return *(Uint32*)p;
			break;
		default:
			return 0;
	}
#endif

    return *(uint32*)((uint8*)pixels + y * trPitch + x * trBpp);
}

