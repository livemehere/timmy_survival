#pragma once

#include "../GameObject.hpp"
#include "raylib.h"
#include "raymath.h"

class LinearMovement : public Component {
public:
  Vector2 dirction;
  float speed;

  LinearMovement(Vector2 dir, float speed) : dirction(dir), speed(speed) {}

  void Update(float dt) override {
    Vector2 movement = Vector2Scale(dirction, speed * dt);
    gameObject->position = Vector2Add(gameObject->position, movement);
  }
};
