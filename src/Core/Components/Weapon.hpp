#pragma once

#include "../Component.hpp"
#include "../Timer.hpp"

class Weapon : public Component {
public:
  Timer timer;
  float damage;

  Weapon(float damage, float cooldown)
      : timer(cooldown, true), damage(damage) {}

  void Update(float dt) override {
    if (timer.Update(dt)) {
      Activate();
    }
  }

  virtual void Activate() = 0;
};
