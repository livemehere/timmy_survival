#pragma once

#include "../Component.hpp"
#include "../GameObject.hpp"
#include "raylib.h"
#include "raymath.h"

class Knockback : public Component {
public:
  Vector2 velocity = {0, 0};
  float friction = 15.0f;

  Knockback() = default;

  void Apply(Vector2 force) { velocity = force; }

  void Update(float dt) override {
    if (velocity.x == 0 && velocity.y == 0)
      return;

    gameObject->position.x += velocity.x * dt;
    gameObject->position.y += velocity.y * dt;

    float force = Vector2Length(velocity);
    if (force > 0) {
      velocity.x = Lerp(velocity.x, 0, friction * dt);
      velocity.y = Lerp(velocity.y, 0, friction * dt);
    }
  }
};
