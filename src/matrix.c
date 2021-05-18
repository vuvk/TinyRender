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

#include <string.h>
#include <math.h>

//#include "SDL2/SDL_stdinc.h"

#include "matrix.h"
#include "types.h"
#include "utils.h"


void LoadIdentity_M3x3(TMatrix3x3 matrix)
{
    matrix[1] = matrix[2] = matrix[3] =
    matrix[5] = matrix[6] = matrix[7] = 0.0f;

    matrix[0] = matrix[4] = matrix[8] = 1.0f;
}



void LoadIdentity_M4x4(TMatrix4x4 matrix)
{
    matrix[1]  = matrix[2]  = matrix[3]  = matrix[4]  =
    matrix[6]  = matrix[7]  = matrix[8]  = matrix[9]  =
    matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0f;

    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

inline void Multiplyf_Scalar(TMatrix4x4 matrix, float scalar)
{
    for (uint8 i = 0; i < 16; ++i)
    {
        matrix[i] *= scalar;
    }
}

void Multiplyf_M4x4(TMatrix4x4 dst, const TMatrix4x4 src1, const TMatrix4x4 src2)
{

    TMatrix4x4 temp = {0};

    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            temp[col * 4 + row] = 0.0f;

            for (int i = 0; i < 4; ++i)
                temp[col * 4 + row] += src1[i * 4 + row] * src2[col * 4 + i];
        }
    }

    memcpy(dst, temp, sizeof(TMatrix4x4));

    /*
    TMatrix4x4 result = {0};

    for (int i = 0; i < 4; ++i)
    {
        result [i + 0 * 3] = src1 [0 + 0 * 3]*src2 [i + 0 * 3] +
                             src1 [1 + 0 * 3]*src2 [i + 1 * 3] +
                             src1 [2 + 0 * 3]*src2 [i + 2 * 3] +
                             src1 [3 + 0 * 3]*src2 [i + 3 * 3];

        result [i + 1 * 3] = src1 [0 + 1 * 3]*src2 [i + 0 * 3] +
                             src1 [1 + 1 * 3]*src2 [i + 1 * 3] +
                             src1 [2 + 1 * 3]*src2 [i + 2 * 3] +
                             src1 [3 + 1 * 3]*src2 [i + 3 * 3];

        result [i + 2 * 3] = src1 [0 + 2 * 3]*src2 [i + 0 * 3] +
                             src1 [1 + 2 * 3]*src2 [i + 1 * 3] +
                             src1 [2 + 2 * 3]*src2 [i + 2 * 3] +
                             src1 [3 + 2 * 3]*src2 [i + 3 * 3];

        result [i + 3 * 3] = src1 [0 + 3 * 3]*src2 [i + 0 * 3] +
                             src1 [1 + 3 * 3]*src2 [i + 1 * 3] +
                             src1 [2 + 3 * 3]*src2 [i + 2 * 3] +
                             src1 [3 + 3 * 3]*src2 [i + 3 * 3];
    }

    memcpy(dst, result, sizeof(TMatrix4x4));
    */
}

TrVector4f Multiplyf_M4x4_V4f(const TMatrix4x4 matrix, const TrVector4f vec)
{
/*
    | a b c d |   | x |     | a*x + b*y + c*z + d*w |
    | e f g h | * | y |  =  | e*x + f*y + g*z + h*w |
    | i j k l |   | z |     | i*x + j*y + k*z + l*w |
    | m n o q |   | w |     | m*x + n*y + o*z + q*w |
*/

    float temp[4] = {0};
    float result[4] = {0};
    int i;

    for (i = 0; i < 4; ++i)
    {
        temp[i] = matrix[i]      * vec.x +
                  matrix[4 + i]  * vec.y +
                  matrix[8 + i]  * vec.z +
                  matrix[12 + i] * vec.w;
    }

    memcpy (result, temp, sizeof (temp));

    if (result[3] != 0.0f && result[3] != 1.0f)
    {
        float w;
        if (temp[3] != 0.0f)
        {
            w = 1.0f / temp[3];
        }
        else
        {
            w = 1.0f;
        }

        for (i = 0; i < 4; i++)
        {
            result[i] *= w;
        }
    }

    return *(TrVector4f*)result;
}

void Translatef_M4x4(TMatrix4x4 matrix, float x, float y, float z)
{
    TMatrix4x4 temp;

    LoadIdentity_M4x4(temp);

    temp[12] = x;
    temp[13] = y;
    temp[14] = z;

    Multiplyf_M4x4(matrix, matrix, temp);
}

void Translatev_M4x4(TMatrix4x4 matrix, TrVector3f translate)
{
    TMatrix4x4 temp;

    LoadIdentity_M4x4(temp);

    temp[12] = translate.x;
    temp[13] = translate.y;
    temp[14] = translate.z;

    Multiplyf_M4x4(matrix, matrix, temp);
}


void Scalef_M4x4(TMatrix4x4 matrix, float x, float y, float z)
{
    TMatrix4x4 temp;

    LoadIdentity_M4x4(temp);

    temp[0]  = x;
    temp[5]  = y;
    temp[10] = z;

    Multiplyf_M4x4(matrix, matrix, temp);
}

void Scalev_M4x4(TMatrix4x4 matrix, TrVector3f scale)
{
    TMatrix4x4 temp;

    LoadIdentity_M4x4(temp);

    temp[0]  = scale.x;
    temp[5]  = scale.y;
    temp[10] = scale.z;

    Multiplyf_M4x4(matrix, matrix, temp);
}

void Rotatef_M4x4(TMatrix4x4 matrix, float angle, float x, float y, float z)
{
    TMatrix4x4 temp;

    float s = sin(angle);
    float c = cos(angle);

    TrVector3f vector = tr_vec3f_norm (tr_vec3f_new (x, y, z));

    float xn = vector.x;
    float yn = vector.y;
    float zn = vector.z;

    LoadIdentity_M4x4(temp);

    temp[0]  = xn * xn * (1 - c) + c;
    temp[1]  = xn * yn * (1 - c) + zn * s;
    temp[2]  = xn * zn * (1 - c) - yn * s;

    temp[4]  = xn * yn * (1 - c) - zn * s;
    temp[5]  = yn * yn * (1 - c) + c;
    temp[6]  = yn * zn * (1 - c) + xn * s;

    temp[8]  = xn * zn * (1 - c) + yn * s;
    temp[9]  = yn * zn * (1 - c) - xn * s;
    temp[10] = zn * zn * (1 - c) + c;

    Multiplyf_M4x4(matrix, matrix, temp);
}

void RotateRxf_M4x4(TMatrix4x4 matrix, float angle)
{
    TMatrix4x4 temp;

    float s = sin(angle);
    float c = cos(angle);

    LoadIdentity_M4x4(temp);

    temp[5]  = c;
    temp[6]  = s;

    temp[9]  = -s;
    temp[10] = c;

    Multiplyf_M4x4(matrix, matrix, temp);
}

void RotateRyf_M4x4(TMatrix4x4 matrix, float angle)
{
    TMatrix4x4 temp;

    float s = sin(angle);
    float c = cos(angle);

    LoadIdentity_M4x4(temp);

    temp[0] = c;
    temp[2] = -s;

    temp[8] = s;
    temp[10] = c;

    Multiplyf_M4x4(matrix, matrix, temp);
}

void RotateRzf_M4x4(TMatrix4x4 matrix, float angle)
{
    TMatrix4x4 temp;

    float s = sin(angle);
    float c = cos(angle);

    LoadIdentity_M4x4(temp);

    temp[0] = c;
    temp[1] = s;

    temp[4] = -s;
    temp[5] = c;

    Multiplyf_M4x4(matrix, matrix, temp);
}



// "camera"
bool Orthof_M4x4(TMatrix4x4 result, const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
    if ((right - left) == 0.0f ||
        (top - bottom) == 0.0f ||
        (zFar - zNear) == 0.0f)
    {
        return false;
    }

    result[1] = result[2] = result[3]  =
    result[4] = result[6] = result[7]  =
    result[8] = result[9] = result[11] = 0.0f;

    result[0]  =  2.0f / (right - left);
    result[5]  =  2.0f / (top - bottom);
    result[10] = -2.0f / (zFar - zNear);
    result[12] = -(right + left) / (right - left);
    result[13] = -(top + bottom) / (top - bottom);
    result[14] = -(zFar + zNear)   / (zFar - zNear);
    result[15] =  1.0f;

    return true;
}

bool Frustumf_M4x4(TMatrix4x4 result, const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
    if ((right - left) == 0.0f ||
        (top - bottom) == 0.0f ||
        (zFar - zNear) == 0.0f)
    {
        return false;
    }

    result[1]  = result[2]  = result[3]  =
    result[4]  = result[6]  = result[7]  =
    result[12] = result[13] = result[15] = 0.0f;

    result[0]  =  2.0f * zNear / (right - left);
    result[5]  =  2.0f * zNear / (top - bottom);
    result[8]  =  (right + left) / (right - left);
    result[9]  =  (top + bottom) / (top - bottom);
    result[10] = -(zFar + zNear) / (zFar - zNear);
    result[11] = -1.0f;
    result[14] = -(2.0f * zFar * zNear) / (zFar - zNear);

    return true;
}

bool Perspectivef_M4x4(TMatrix4x4 result, const float fovy, const float aspect, const float zNear, const float zFar)
{
    if (fovy <= 0.0f || fovy >= 180.0f)
    {
        return false;
    }

    float xmin, xmax, ymin, ymax;
    ymax =  zNear * tan(fovy * M_PI / 360.0f);
    ymin = -ymax;
    xmin =  ymin * aspect;
    xmax =  ymax * aspect;

    return Frustumf_M4x4(result, xmin, xmax, ymin, ymax, zNear, zFar);
}

void LookAtf_M4x4(TMatrix4x4 result, const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ)
{
    TrVector3f forward = tr_vec3f_new (
        centerX - eyeX,
        centerY - eyeY,
        centerZ - eyeZ
    );
    forward = tr_vec3f_norm (forward);

    TrVector3f up = tr_vec3f_new (upX, upY, upZ);

    TrVector3f side = tr_vec3f_norm (tr_vec3f_cross (forward, up));

    up = tr_vec3f_cross (side, forward);

    result[3] = result[7] = result[11] = result[12] =
    result[13] = result[14] = 0.0f;

    result[0] =  side.x;
    result[1] =  up.x;
    result[2] = -forward.x;
    result[4] =  side.y;
    result[5] =  up.y;
    result[6] = -forward.y;
    result[8] =  side.z;
    result[9] =  up.z;
    result[10] = -forward.z;
    result[15] = 1.0f;

    Translatef_M4x4(result, -eyeX, -eyeY, -eyeZ);
}

inline void LookAtv_M4x4(TMatrix4x4 result, const TrVector3f eye, const TrVector3f center, const TrVector3f up)
{
    LookAtf_M4x4(result, eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}


