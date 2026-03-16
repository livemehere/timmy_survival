#pragma once

#include "../../Core/Timer.hpp"
#include "../../Core/Weapon.hpp"
#include "../../Prefabs/Types/ProjectileDefinition.hpp"

class FireWeapon : public Weapon {
public:
  Timer timer;
  float projectileSpeed;
  float projectileLifetime;
  float projectileRadius;
  float range;
  bool showRange = false;

  ProjectileDefinition projectileDefinition;

  FireWeapon(float damage, float cooldown, float range,
             const ProjectileDefinition &projectileDefinition)
      : Weapon(damage), timer(cooldown, true),
        projectileSpeed(projectileDefinition.speed),
        projectileLifetime(projectileDefinition.lifetime),
        projectileRadius(projectileDefinition.colliderRadius), range(range),
        projectileDefinition(projectileDefinition) {}

  void Update(float dt) override;

  void Activate() override;

  GameObject *FindNearestEnemy();

  void Draw() override;
};
