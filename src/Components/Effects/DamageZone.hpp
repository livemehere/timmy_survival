#pragma once

#include "../../Core/Effect.hpp"
#include "../../Core/Timer.hpp"

class DamageZone : public Effect {
public:
  float damage;
  float width;
  float height;
  float knockbackForce;
  float tickInterval;
  Timer lifetimeTimer;

  DamageZone(float damage, float width, float height, float lifetime,
             float knockbackForce, float tickInterval)
      : damage(damage), width(width), height(height),
        knockbackForce(knockbackForce),
        tickInterval(tickInterval), lifetimeTimer(lifetime, false) {}

  void Update(float dt) override;
  void Draw() override;
  void Trigger() override;
};
