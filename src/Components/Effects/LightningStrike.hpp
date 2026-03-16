#pragma once

#include "../../Core/Effect.hpp"
#include "../../Core/Timer.hpp"

class LightningStrike : public Effect {
public:
  float damage;
  float radius;
  float knockbackForce;
  float hitCooldown;
  Timer hitTimer;
  Timer lifetimeTimer;
  bool didStrike = false;

  LightningStrike(float damage, float radius, float hitDelay, float lifetime,
                  float knockbackForce, float hitCooldown)
      : damage(damage), radius(radius), knockbackForce(knockbackForce),
        hitCooldown(hitCooldown), hitTimer(hitDelay, false),
        lifetimeTimer(lifetime, false) {}

  void Update(float dt) override;
  void Draw() override;
  void Trigger() override;
};
