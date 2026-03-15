#pragma once
#include "../Core/Collider.hpp"
#include "../Core/GameObject.hpp"
#include "raymath.h"

class CircleCollider : public Collider {
public:
  float radius;

  CircleCollider(float r, Vector2 offset = {0, 0});

  Vector2 GetCenter() const;

  void Draw() override;
};
