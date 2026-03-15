#pragma once

#include "../Core/Component.hpp"
#include "../Core/Timer.hpp"
#include "../Utils/MathUtils.hpp"
#include "raylib.h"

class Particle : public Component {
public:
  Vector2 velocity;
  Timer lifeTimer;
  Color color;
  float size;
  float friction;
  float rotation;

  Particle(Vector2 velocity, float lifetime, Color color, float size,
           float friction)
      : velocity(velocity), lifeTimer(lifetime, false), color(color),
        size(size), friction(friction),
        rotation(MathUtils::GetRandom(0.0f, 360.0f)) {}

  void Update(float dt) override;

  void Draw() override;
};
