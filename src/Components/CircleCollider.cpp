#include "CircleCollider.hpp"
#include "raylib.h"

CircleCollider::CircleCollider(float r, Vector2 offset)
    : Collider(ColliderType::CIRCLE, offset), radius(r) {}

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
