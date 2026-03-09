#include "MathUtils.hpp"
#include <cmath>
#include <random>

namespace MathUtils {
static std::random_device rd;
static std::mt19937 gen(rd());

float GetRandom(float min, float max) {
  std::uniform_real_distribution<float> dist(min, max);
  return dist(gen);
}

int GetRandom(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(gen);
}

Vector2 GetRandomArroundPosition(Vector2 from, float minRadius,
                                 float maxRadius) {
  float radian = GetRandom(0.0f, PI * 2.0f);
  float distance = GetRandom(minRadius, maxRadius);

  return {from.x + std::cos(radian) * distance,
          from.y + std::sin(radian) * distance};
}

} // namespace MathUtils
