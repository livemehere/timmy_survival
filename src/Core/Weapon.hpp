#pragma once

#include "Component.hpp"

class Weapon : public Component {
public:
  float damage;

  explicit Weapon(float damage) : damage(damage) {}

  void Update(float dt) override = 0;

  virtual void Activate() = 0;
};
