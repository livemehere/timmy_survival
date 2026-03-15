#pragma once
#include "../Core/Collider.hpp"
#include "raylib.h"

class BoxCollider : public Collider {
public:
  Vector2 size;
  Vector2 offset;

  BoxCollider(float width, float height, Vector2 offset = {0, 0});

  Rectangle GetRect();
  void Draw() override;
};
