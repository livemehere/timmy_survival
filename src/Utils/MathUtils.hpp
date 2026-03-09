#pragma once
#include "raylib.h"

namespace MathUtils {
Vector2 GetRandomArroundPosition(Vector2 from, float minRadius,
                                 float maxRadius);
float GetRandom(float min, float max);
int GetRandom(int min, int max);

} // namespace MathUtils
