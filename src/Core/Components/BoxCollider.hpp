#pragma once
#include "../Component.hpp"
#include "raylib.h"

class BoxCollider : public Component {
public:
  Vector2 size;
  Vector2 offset;

  BoxCollider(float width, float height, float offsetX = 0.0f,
              float offsetY = 0.0f) {
    size = {width, height};
    offset = {offsetX, offsetY};
  }

  Rectangle GetRect();
  void Draw() override;
};
