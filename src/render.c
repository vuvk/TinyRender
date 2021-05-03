#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "SDL2/SDL.h"

//#include "threadpool/threadpool.h"
//#include "tinycthread/tinycthread.h"

#include "utils.h"
#include "matrix.h"
#include "draw.h"
#include "render.h"
#include "render_private.h"

SDL_Window*   window;
/*
SDL_Surface*  windowSurface;
SDL_Renderer* renderer;
SDL_Surface*  frameBuffer;
SDL_Texture*  screen;
*/
SDL_Surface*  frameBuffer;

int trWidth, trHeight, trBits, trBpp, trPitch;
int trHalfWidth, trHalfHeight;

float trNearPlane, trFarPlane;

uint32 clearColor;

TMatrix4x4 proj;
TMatrix4x4 modl;

TMatrix4x4 transform;

float* curMatrix = NULL;   // какая матрица выбрана?

//threadpool_t* threadPool;
//mtx_t renderLock;

bool trInit(const char* title, int width, int height, int bits)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "ERROR: cannot initialize SDL video.\n");
        return false;
    }

#ifdef USE_SDL1
    frameBuffer = SDL_SetVideoMode(width, height, bits, /*SDL_HWSURFACE | SDL_DOUBLEBUF*/0);
#else  // USE_SDL2
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    frameBuffer = SDL_GetWindowSurface(window);
#endif // USE_SDL1
    if (frameBuffer == NULL)
    {
        fprintf(stderr, "SDL_SetVideoMode() failed: %s\n", SDL_GetError());
        return false;
    }

    trBits   = frameBuffer->format->BitsPerPixel;
    trBpp    = frameBuffer->format->BytesPerPixel;
    trWidth  = width;
    trHeight = height;
    trPitch  = frameBuffer->pitch;
    trHalfWidth  = width  >> 1;
    trHalfHeight = height >> 1;
    trNearPlane = 0.1;
    trFarPlane  = 100.0;

    clearColor = 0xFF;

    //assert((threadPool = threadpool_create(THREAD, QUEUE, 0)) != NULL);
    //mtx_init(&renderLock, 0);

    /*
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "0");
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    */

    return true;
}

void trClose()
{
    SDL_FreeSurface(frameBuffer);
    /*
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    */
    SDL_DestroyWindow(window);
    SDL_Quit();

    //threadpool_destroy(threadPool, 0);
    //mtx_destroy(&renderLock);
}

inline void trClearColorb(uint8 r, uint8 g, uint8 b)
{
    clearColor = RGB_TO_COLOR(r, g, b);
}

inline void trClearColori(uint32 color)
{
    clearColor = color;
}

inline void trClear()
{
    //DrawRect(0, 0, trWidth, trHeight, clearColor, frameBuffer->pixels);
    ClearPixels(clearColor, trWidth, trHeight, frameBuffer->pixels);
}

void trSwapBuffers()
{
    SDL_UpdateWindowSurface(window);
}


/* MATRIX */
void trMatrixMode(uint8 mode)
{
    switch (mode)
    {
        case 1 :
            curMatrix = (float*)proj;
            break;

        case 2 :
            curMatrix = (float*)modl;
            break;
    }
}

void trLoadIdentity()
{
    if (curMatrix == NULL)
        return;

    LoadIdentity_M4x4(curMatrix);
}

bool trOrthof(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
    if (curMatrix == NULL)
        return false;

    return Orthof_M4x4(curMatrix, left, right, bottom, top, zNear, zFar);
}

bool trFrustumf(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
    if (curMatrix == NULL)
        return false;

    return Frustumf_M4x4(curMatrix, left, right, bottom, top, zNear, zFar);
}

bool trPerspectivef(const float fovy, const float aspect, const float zNear, const float zFar)
{
    if (curMatrix == NULL)
        return false;

    trNearPlane = zNear;
    trFarPlane  = zFar;

    return Perspectivef_M4x4(curMatrix, fovy, aspect, zNear, zFar);
}

void trLookAtf(const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ)
{
    if (curMatrix == NULL)
        return;

    LookAtf_M4x4(curMatrix, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void trLookAtv(const float* eye, const float* center, const float* up)
{
    if (curMatrix == NULL)
        return;

    LookAtf_M4x4(curMatrix, eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}

inline void trTransformsPrepare()
{
    //Multiplyf_M4x4(transform, modl, proj);
    Multiplyf_M4x4(transform, proj, modl);
}

/* TRANSFORMATIONS */
void trRotatef(float angle, float x, float y, float z)
{
    if (curMatrix == NULL)
        return;

    Rotatef_M4x4(curMatrix, angle, x, y, z);
}

void trRotatefv(float angle, float* v)
{
    if (curMatrix == NULL)
        return;

    Rotatef_M4x4(curMatrix, angle, v[0], v[1], v[2]);
}

void trRotateXf(float angle)
{
    if (curMatrix == NULL)
        return;

    RotateRxf_M4x4(curMatrix, angle);
}

void trRotateYf(float angle)
{
    if (curMatrix == NULL)
        return;

    RotateRyf_M4x4(curMatrix, angle);
}

void trRotateZf(float angle)
{
    if (curMatrix == NULL)
        return;

    RotateRzf_M4x4(curMatrix, angle);
}


/* DRAWING */
static bool PointPrepare(SVector3f v3f, SVector2i* out)
{
    SVector4f v4f = Vector3fToVector4f(v3f);

    v4f = Multiplyf_M4x4_V4f(transform, v4f);
    v3f = Vector4fToVector3f(v4f);

    SVector2f v2f = Vector3fToVector2f(v3f);

    int x0 = (int32)( v2f.x * trHalfWidth  + trHalfWidth);
    int y0 = (int32)(-v2f.y * trHalfHeight + trHalfHeight); // оси перевернуть!
    *out = NewVector2i(x0, y0);

    // точка за плоскостями ближней и дальней
    if (v3f.z < trNearPlane || v3f.z > trFarPlane)
        return false;

    // точка не попала на экран
    if ((v2f.x < -1.0) || (v2f.x > 1.0) ||
        (v2f.y < -1.0) || (v2f.y > 1.0))
        return false;

    return true;
}


inline void trPoint2i(int32 x, int32 y, uint32 color)
{
    PutPixel(x, y, color, frameBuffer->pixels);
}

inline void trPoint2iv(int32* v, uint32 color)
{
    PutPixel(v[0], v[1], color, frameBuffer->pixels);
}

void trPoint3f(float x, float y, float z, uint32 color)
{
    SVector2i point;
    if (PointPrepare(NewVector3f(x, y, z), &point))
        PutPixel(point.x, point.y, color, frameBuffer->pixels);
}

inline void trPoint3fv(float* v, uint32 color)
{
    trPoint3f(v[0], v[1], v[2], color);
}

inline void trLine2i(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color)
{
    DrawLine(x0, y0, x1, y1, color, frameBuffer->pixels);
}

inline void trLine2iv(int32* v0, int32* v1, uint32 color)
{
    DrawLine(v0[0], v0[1], v1[0], v1[1], color, frameBuffer->pixels);
}

void trLineBicolor2i(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color0, uint32 color1)
{
    if (color0 != color1)
        DrawLineBicolor(x0, y0, x1, y1, color0, color1, frameBuffer->pixels);
    else
        DrawLine(x0, y0, x1, y1, color0, frameBuffer->pixels);
}

inline void trLineBicolor2iv(int32* v0, int32* v1, uint32 color0, uint32 color1)
{
    trLineBicolor2i(v0[0], v0[1], v1[0], v1[1], color0, color1);
}

void trLine3f(float x0, float y0, float z0, float x1, float y1, float z1, uint32 color)
{
    SVector2i p0;
    SVector2i p1;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);

    if (pointVis0 || pointVis1)
        trLine2iv((int32*)&p0, (int32*)&p1, color);
}

inline void trLine3fv(float* v0, float* v1, uint32 color)
{
    trLine3f(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], color);
}

void trLineBicolor3f(float x0, float y0, float z0, float x1, float y1, float z1, uint32 color0, uint32 color1)
{
    SVector2i p0;
    SVector2i p1;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);

    if (pointVis0 || pointVis1)
        trLineBicolor2iv((int32*)&p0, (int32*)&p1, color0, color1);
}

inline void trLineBicolor3fv(float* v0, float* v1, uint32 color0, uint32 color1)
{
    trLineBicolor3f(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], color0, color1);
}

inline void trRect(int32 x, int32 y, uint32 w, uint32 h, uint32 color)
{
    DrawRect(x, y, w, h, color, frameBuffer->pixels);
}

inline void trTriangleColor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color)
{
    DrawTriangleColor(x0, y0, x1, y1, x2, y2, color, frameBuffer->pixels);
}

inline void trTriangleColor2iv(int32* v0, int32* v1, int32* v2, uint32 color)
{
    DrawTriangleColor(v0[0], v0[1], v1[0], v1[1], v2[0], v2[1], color, frameBuffer->pixels);
}

void trTriangleTexture2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2,
                         float u0, float v0, float u1, float v1, float u2, float v2,
                         STexture* texture)
{
    if (texture == NULL || texture->pixels == NULL)
        return;

    DrawTriangleTextured(x0, y0, x1, y1, x2, y2, u0, v0, u1, v1, u2, v2, texture, frameBuffer->pixels);
}

inline void trTriangleTexture2iv(int32* v0, int32* v1, int32* v2, float* uv0, float* uv1, float* uv2, STexture* texture)
{
    trTriangleTexture2i( v0[0],  v0[1],  v1[0],  v1[1],  v2[0],  v2[1],
                        uv0[0], uv0[1], uv1[0], uv1[1], uv2[0], uv2[1],
                        texture);
}

inline void trTriangleTricolor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color0, uint32 color1, uint32 color2)
{
    DrawTriangleTricolor(x0, y0, x1, y1, x2, y2, color0, color1, color2, frameBuffer->pixels);
}

inline void trTriangleTricolor2iv(int32* v0, int32* v1, int32* v2, uint32 color0, uint32 color1, uint32 color2)
{
    DrawTriangleTricolor(v0[0], v0[1], v1[0], v1[1], v2[0], v2[1], color0, color1, color2, frameBuffer->pixels);
}

void trTriangleColor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, uint32 color)
{
    SVector2i p0;
    SVector2i p1;
    SVector2i p2;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);
    bool pointVis2 = PointPrepare(NewVector3f(x2, y2, z2), &p2);

    if (pointVis0 || pointVis1 || pointVis2)
        trTriangleColor2iv((int32*)&p0, (int32*)&p1, (int32*)&p2, color);
}

inline void trTriangleColor3fv(float* v0, float* v1, float* v2, uint32 color)
{
    trTriangleColor3f(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color);
}

void trTriangleTricolor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, uint32 color0, uint32 color1, uint32 color2)
{
    SVector2i p0;
    SVector2i p1;
    SVector2i p2;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);
    bool pointVis2 = PointPrepare(NewVector3f(x2, y2, z2), &p2);

    if (pointVis0 || pointVis1 || pointVis2)
        trTriangleTricolor2iv((int32*)&p0, (int32*)&p1, (int32*)&p2, color0, color1, color2);
}

inline void trTriangleTricolor3fv(float* v0, float* v1, float* v2, uint32 color0, uint32 color1, uint32 color2)
{
    trTriangleTricolor3f(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color0, color1, color2);
}

void trTriangleTexture3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2,
                         float u0, float v0, float u1, float v1, float u2, float v2,
                         STexture* texture)
{
    SVector2i p0;
    SVector2i p1;
    SVector2i p2;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);
    bool pointVis2 = PointPrepare(NewVector3f(x2, y2, z2), &p2);

    if (pointVis0 || pointVis1 || pointVis2)
        DrawTriangleTextured(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y,
                             u0,   v0,   u1,   v1,   u2,   v2,
                             texture, frameBuffer->pixels);
}

inline void trTriangleTexture3fv(float*  v0, float*  v1, float*  v2,
                                 float* uv0, float* uv1, float* uv2,
                                 STexture* texture)
{
    trTriangleTexture3f( v0[0],  v0[1], v0[1],  v1[0],  v1[1], v1[2],  v2[0],  v2[1], v2[2],
                        uv0[0], uv0[1],        uv1[0], uv1[1],        uv2[0], uv2[1],
                        texture);
}

inline void trQuadColor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, uint32 color)
{
    DrawQuadColor(x0, y0, x1, y1, x2, y2, x3, y3, color, frameBuffer->pixels);
}

inline void trQuadColor2iv(int32* v0, int32* v1, int32* v2, int32* v3, uint32 color)
{
    DrawQuadColor(v0[0], v0[1], v1[0], v1[1], v2[0], v2[1], v3[0], v3[1], color, frameBuffer->pixels);
}

void trQuadTexture2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3,
                     float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                     STexture* texture)
{
    if (texture == NULL || texture->pixels == NULL)
        return;

    DrawQuadTextured(x0, y0, x1, y1, x2, y2, x3, y3, u0, v0, u1, v1, u2, v2, u3, v3, texture, frameBuffer->pixels);
}

inline void trQuadTexture2iv(int32*  v0, int32*  v1, int32*  v2, int32*  v3,
                             float* uv0, float* uv1, float* uv2, float* uv3,
                             STexture* texture)
{
    trQuadTexture2i( v0[0],  v0[1],  v1[0],  v1[1],  v2[0],  v2[1],  v3[0],  v3[1],
                    uv0[0], uv0[1], uv1[0], uv1[1], uv2[0], uv2[1], uv3[0], uv3[1],
                    texture);
}

void trQuadColor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, uint32 color)
{
    SVector2i p0;
    SVector2i p1;
    SVector2i p2;
    SVector2i p3;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);
    bool pointVis2 = PointPrepare(NewVector3f(x2, y2, z2), &p2);
    bool pointVis3 = PointPrepare(NewVector3f(x3, y3, z3), &p3);

    if (pointVis0 || pointVis1 || pointVis2 || pointVis3)
        trQuadColor2iv((int32*)&p0, (int32*)&p1, (int32*)&p2, (int32*)&p3, color);
}

inline void trQuadColor3fv(float* v0, float* v1, float* v2, float* v3, uint32 color)
{
    trQuadColor3f(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2], color);
}


inline void trQuadFourcolor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, uint32 color0, uint32 color1, uint32 color2, uint32 color3)
{
    DrawQuadFourcolor(x0, y0, x1, y1, x2, y2, x3, y3, color0, color1, color2, color3, frameBuffer->pixels);
}

inline void trQuadFourcolor2iv(int32* v0, int32* v1, int32* v2, int32* v3, uint32 color0, uint32 color1, uint32 color2, uint32 color3)
{
    DrawQuadFourcolor(v0[0], v0[1], v1[0], v1[1], v2[0], v2[1], v3[0], v3[1], color0, color1, color2, color3, frameBuffer->pixels);
}

void trQuadFourcolor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, uint32 color0, uint32 color1, uint32 color2, uint32 color3)
{
    SVector2i p0;
    SVector2i p1;
    SVector2i p2;
    SVector2i p3;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);
    bool pointVis2 = PointPrepare(NewVector3f(x2, y2, z2), &p2);
    bool pointVis3 = PointPrepare(NewVector3f(x3, y3, z3), &p3);

    if (pointVis0 || pointVis1 || pointVis2 || pointVis3)
        trQuadFourcolor2iv((int32*)&p0, (int32*)&p1, (int32*)&p2, (int32*)&p3, color0, color1, color2, color3);
}

inline void trQuadFourcolor3fv(float* v0, float* v1, float* v2, float* v3, uint32 color0, uint32 color1, uint32 color2, uint32 color3)
{
    trQuadFourcolor3f(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2], color0, color1, color2, color3);
}

void trQuadTexture3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3,
                     float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                     STexture* texture)
{
    SVector2i p0;
    SVector2i p1;
    SVector2i p2;
    SVector2i p3;
    bool pointVis0 = PointPrepare(NewVector3f(x0, y0, z0), &p0);
    bool pointVis1 = PointPrepare(NewVector3f(x1, y1, z1), &p1);
    bool pointVis2 = PointPrepare(NewVector3f(x2, y2, z2), &p2);
    bool pointVis3 = PointPrepare(NewVector3f(x3, y3, z3), &p3);

    if (pointVis0 || pointVis1 || pointVis2 || pointVis3)
        DrawQuadTextured(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y,
                         u0,   v0,   u1,   v1,   u2,   v2,   u3,   v3,
                         texture, frameBuffer->pixels);
}

void trQuadTexture3fv(float*  v0, float*  v1, float*  v2, float*  v3,
                      float* uv0, float* uv1, float* uv2, float* uv3,
                      STexture* texture)
{
    trQuadTexture3f( v0[0],  v0[1], v0[1],  v1[0],  v1[1], v1[2],  v2[0],  v2[1], v2[2],  v3[0],  v3[1], v3[2],
                    uv0[0], uv0[1],        uv1[0], uv1[1],        uv2[0], uv2[1],        uv3[0], uv3[1],
                    texture);
}

