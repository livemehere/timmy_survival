#pragma once
#include "Collider.hpp"
#include "raylib.h"

class BoxCollider : public Collider {
public:
  Vector2 size;
  Vector2 offset;

  BoxCollider(float width, float height, Vector2 offset = {0, 0})
      : Collider(ColliderType::BOX, offset) {
    size = {width, height};
  }

  Rectangle GetRect();
  void Draw() override;
};
