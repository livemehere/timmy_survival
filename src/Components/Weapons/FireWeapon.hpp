#pragma once

#include "../../Core/Timer.hpp"
#include "../../Core/Weapon.hpp"
#include "../Render/SpriteRenderer.hpp"

class FireWeapon : public Weapon {
public:
  Timer timer;
  float projectileSpeed;
  float projectileLifetime;
  float projectileRadius;
  float range;
  bool showRange = false;

  AnimationConfig projectileAnimConfig;
  Vector2 projectileAnimScale = {1.0f, 1.0f};

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
