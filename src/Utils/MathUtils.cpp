#include "MathUtils.hpp"
#include <cmath>

namespace MathUtils {
Vector2 GetRandomArroundPosition(Vector2 from, float minRadius,
                                 float maxRadius) {
  float radian = GetRandomValue(0, 360) * DEG2RAD;
  float distance = GetRandomValue(minRadius, maxRadius);

  return {from.x + cos(radian) * distance, from.y + sin(radian) * distance};
};

} // namespace MathUtils
