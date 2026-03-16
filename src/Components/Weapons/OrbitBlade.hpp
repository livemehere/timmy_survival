#pragma once

#include "../../Core/Component.hpp"

class OrbitBlade : public Component {
public:
  float damage;
  float knockbackForce;
  float hitCooldown;

  OrbitBlade(float damage, float knockbackForce, float hitCooldown)
      : damage(damage), knockbackForce(knockbackForce),
        hitCooldown(hitCooldown) {}

  void OnTriggerEnter(Collider *other) override;
};
