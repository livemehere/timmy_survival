#pragma once

#include "../../Core/Timer.hpp"
#include "../../Core/Weapon.hpp"

class FireWeapon : public Weapon {
public:
  Timer timer;
  float projectileSpeed;
  float projectileLifetime;
  float projectileRadius;
  float range;
  bool showRange = false;

  FireWeapon(float damage, float cooldown, float projectileSpeed,
             float projectileLifetime, float projectileRadius, float range)
      : Weapon(damage), timer(cooldown, true), projectileSpeed(projectileSpeed),
        projectileLifetime(projectileLifetime),
        projectileRadius(projectileRadius), range(range) {}

  void Update(float dt) override;

  void Activate() override;

  GameObject *FindNearestEnemy();

  void Draw() override;
};
