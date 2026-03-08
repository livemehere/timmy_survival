#pragma once
#include "../GameObject.hpp"
#include "Collider.hpp"
#include "raymath.h"

class CircleCollider : public Collider {
public:
  float radius;

  CircleCollider(float r, Vector2 offset = {0, 0})
      : Collider(ColliderType::CIRCLE, offset), radius(r) {}

  Vector2 GetCenter() const { return Vector2Add(gameObject->position, offset); }

  void Draw() override;
};
