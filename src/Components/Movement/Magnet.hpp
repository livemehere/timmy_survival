#pragma once

#include "../../Core/Component.hpp"
#include "raylib.h"
#include <functional>

class Magnet : public Component {
public:
  Vector2 *targetPos = nullptr;
  float speed = 200.0f;

  Magnet() = default;

  std::function<void()> onCollect;

  void Update(float dt) override;
};
