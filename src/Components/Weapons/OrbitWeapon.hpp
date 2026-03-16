#pragma once

#include "../../Core/Weapon.hpp"

class OrbitWeapon : public Weapon {
public:
  GameObject *owner;
  float orbitRadius;
  float orbitSpeedDegrees;
  float angleDegrees;

  OrbitWeapon(GameObject *owner, float damage, float orbitRadius,
              float orbitSpeedDegrees, float startAngleDegrees)
      : Weapon(damage), owner(owner), orbitRadius(orbitRadius),
        orbitSpeedDegrees(orbitSpeedDegrees), angleDegrees(startAngleDegrees) {}

  void Start() override;

  void Update(float dt) override;

  void Activate() override {}

  void OnTriggerEnter(Collider *other) override;
};
