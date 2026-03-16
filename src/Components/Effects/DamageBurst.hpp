#pragma once

#include "../../Core/Effect.hpp"
#include "../../Core/Timer.hpp"

class DamageBurst : public Effect {
public:
  float damage;
  float width;
  float height;
  float knockbackForce;
  float hitCooldown;
  Timer hitTimer;
  Timer lifetimeTimer;
  bool didBurst = false;

  DamageBurst(float damage, float width, float height, float hitDelay,
              float lifetime, float knockbackForce, float hitCooldown)
      : damage(damage), width(width), height(height),
        knockbackForce(knockbackForce), hitCooldown(hitCooldown),
        hitTimer(hitDelay, false), lifetimeTimer(lifetime, false) {}

  void Update(float dt) override;
  void Draw() override;
  void Trigger() override;
};
