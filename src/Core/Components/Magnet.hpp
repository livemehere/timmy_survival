#pragma once

#include "../GameObject.hpp"
#include "raylib.h"
#include "raymath.h"
#include <functional>

class Magnet : public Component {
public:
  Vector2 *targetPos = nullptr;
  float speed = 200.0f;

  Magnet() = default;

  std::function<void()> onCollect;

  void Update(float dt) override {
    if (!targetPos)
      return;

    Vector2 dir = Vector2Subtract(*targetPos, gameObject->position);
    float distance = Vector2Length(dir);

    if (distance > 1.0f) {
      Vector2 normDir = Vector2Normalize(dir);
      gameObject->position =
          Vector2Add(gameObject->position, Vector2Scale(normDir, speed * dt));
    } else {
      if (onCollect) {
        onCollect();
      }
    }
  }
};
