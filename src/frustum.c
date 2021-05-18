/**

   Copyright 2021 Anton "Vuvk" Shcherbatykh (vuvk69@gmail.com)

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

#include <math.h>
#include "render.h"
#include "matrix.h"
#include "frustum.h"

// Хранит A B C и D переменные для каждой стороны пирамиды.
float frustum[6][4];

// обозначение сторон пирамиды
typedef enum
{
    RIGHT,
    LEFT,
    BOTTOM,
    TOP,
    BACK,
    FRONT
} TrFrustumSide;

// плоскости
typedef enum
{
    A,  // X
    B,  // Y
    C,  // Z
    D   // расстояние от начала координат
} TrPlaneData;

/** Нормализует плоскость (сторону) переданного frustum */
static void tr_plane_normalize (int side)
{
    // Вычисляем величину нормали плоскости (точку A B C)
    // Помните, что (A, B, C) плоскости - то же самое, что (X, Y, Z) для нормали.
    // Чтобы вычислить величину, используем формулу: sqrt(x^2 + y^2 + z^2)
    float magnitude = (float) sqrt (
        frustum[side][A] * frustum[side][A] +
        frustum[side][B] * frustum[side][B] +
        frustum[side][C] * frustum[side][C]
    );

    // Затем делим значения плоскости на её величину.
    // После этого с плоскостью будет легче работать.
    float inv_magnitude = (magnitude != 0.0f) ? 1.0f / magnitude : 0.0f;
    frustum[side][A] *= inv_magnitude;
    frustum[side][B] *= inv_magnitude;
    frustum[side][C] *= inv_magnitude;
    frustum[side][D] *= inv_magnitude;
}

/** Вычисляет frustum из матриц проекции и моделей */
void tr_frustum_update ()
{
    TMatrix4x4 proj;
    TMatrix4x4 modl;
    TMatrix4x4 clip;   // Плоскости обрезания

    tr_get_matrix (PROJECTION_MATRIX, proj);
    tr_get_matrix (MODELVIEW_MATRIX, modl);

    // Теперь, имея матрицы проекции и моделей, если мы их скомбинируем, то получим плоскости
    // отсечения. Для этого мы умножим матрицы друг на друга.
    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

    // Теперь нам нужно получить стороны frustum’а. Чтобы сделать это,
    // возьмём обрезающие плоскости, полученные выше, и из них получим стороны.
    //
    // получаем ПРАВУЮ сторону frustum’а
    frustum[RIGHT][A] = clip[ 3] - clip[ 0];
    frustum[RIGHT][B] = clip[ 7] - clip[ 4];
    frustum[RIGHT][C] = clip[11] - clip[ 8];
    frustum[RIGHT][D] = clip[15] - clip[12];

    // Теперь, имея нормаль (A,B,C) и расстояние (D) к плоскости,
    // нам нужно нормализовать нормаль и дистанцию
    
    // Нормализуем правую сторону
    tr_plane_normalize (RIGHT);
    
    // получаем ЛЕВУЮ сторону frustum’а
    frustum[LEFT][A] = clip[ 3] + clip[ 0];
    frustum[LEFT][B] = clip[ 7] + clip[ 4];
    frustum[LEFT][C] = clip[11] + clip[ 8];
    frustum[LEFT][D] = clip[15] + clip[12];
    
    // Нормализуем ЛЕВУЮ сторону
    tr_plane_normalize (LEFT);

    // получаем нижнюю сторону frustum’а
    frustum[BOTTOM][A] = clip[ 3] + clip[ 1];
    frustum[BOTTOM][B] = clip[ 7] + clip[ 5];
    frustum[BOTTOM][C] = clip[11] + clip[ 9];
    frustum[BOTTOM][D] = clip[15] + clip[13];

    // Нормализуем нижнюю сторону
    tr_plane_normalize (BOTTOM);

    // получаем верхнюю сторону frustum’а
    frustum[TOP][A] = clip[ 3] - clip[ 1];
    frustum[TOP][B] = clip[ 7] - clip[ 5];
    frustum[TOP][C] = clip[11] - clip[ 9];
    frustum[TOP][D] = clip[15] - clip[13];
    
    // Нормализуем верхнюю сторону
    tr_plane_normalize (TOP);
    
    // получаем заднюю сторону frustum’а
    frustum[BACK][A] = clip[ 3] - clip[ 2];
    frustum[BACK][B] = clip[ 7] - clip[ 6];
    frustum[BACK][C] = clip[11] - clip[10];
    frustum[BACK][D] = clip[15] - clip[14];
    
    // Нормализуем заднююсторону
    tr_plane_normalize (BACK);
    
    // получаем переднюю сторону frustum’а
    frustum[FRONT][A] = clip[ 3] + clip[ 2];
    frustum[FRONT][B] = clip[ 7] + clip[ 6];
    frustum[FRONT][C] = clip[11] + clip[10];
    frustum[FRONT][D] = clip[15] + clip[14];
    
    // Нормализуем переднюю сторону
    tr_plane_normalize (FRONT);
}

bool tr_frustum_has_pointf (float x, float y, float z)
{ 
    // Проходим через все стороны пирамиды.
    for (int i = 0; i < 6; ++i)
    {
        // Применяем формулу плоскости и проверяем, находится ли точка позади плоскости.
        // Если она позади хотя бы одной плоскости из всех, можно возвращать false.
        if (frustum[i][A] * x + frustum[i][B] * y + frustum[i][C] * z + frustum[i][D] <= 0.0f)
        {
            // Точка находится позади стороны, так что она НЕ внутри пирамиды
            return false;
        }
    }
    
    // Иначе точка находится внутри пирамиды, возвращаем true
    return true;
}

inline bool tr_frustum_has_point (TrVector3f point)
{
    return tr_frustum_has_pointf (point.x, point.y, point.z);
}

bool tr_frustum_has_spheref (float x, float y, float z, float radius)
{

}

inline bool tr_frustum_has_sphere (TrVector3f center, float radius)
{
    return tr_frustum_has_spheref (center.x, center.y, center.z, radius);
}

bool tr_frustum_has_cubef (float x, float y, float z, float radius)
{

}

inline bool tr_frustum_has_cube (TrVector3f center, float radius)
{
    return tr_frustum_has_cubef (center.x, center.y, center.z, radius);
}

