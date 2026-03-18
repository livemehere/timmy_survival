#pragma once

#include "../../Core/Component.hpp"
#include "raylib.h"
#include <functional>

class Magnet : public Component {
public:
  Vector2 *targetPos = nullptr;
  float speed = 200.0f;
  float acceleration = 900.0f;
  float maxSpeed = 720.0f;
  bool isMagnetizing = false;

  Magnet() = default;

  std::function<void()> onCollect;

  void Update(float dt) override;
};
