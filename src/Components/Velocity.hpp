#pragma once

#include "../Core/Component.hpp"
#include "raylib.h"

class Velocity : public Component {
public:
  Vector2 velocity;
  float damping;

  Velocity(Vector2 velocity = {0.0f, 0.0f}, float damping = 0.0f);

  void Apply(Vector2 force);

  void Update(float dt) override;
};
