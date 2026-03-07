#include "CircleCollider.hpp"
#include "raylib.h"

void CircleCollider::Draw() {
  Vector2 center = GetCenter();
  DrawCircleV(center, radius, Fade(RED, 0.5f));
  DrawCircleLinesV(center, radius, RED);
}
