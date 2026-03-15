#include "CircleCollider.hpp"

#include "../../Core/GameObject.hpp"
#include "raylib.h"
#include "raymath.h"

Vector2 CircleCollider::GetCenter() const {
  return Vector2Add(gameObject->position, offset);
}

void CircleCollider::Draw() {
  if (!Collider::debugMode)
    return;

  Vector2 center = GetCenter();
  DrawCircleV(center, radius, Fade(RED, 0.02f));
  DrawCircleLinesV(center, radius, RED);
}
