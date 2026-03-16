#pragma once

#include "../../Core/Weapon.hpp"
#include "../../Prefabs/Types/SpritePreset.hpp"
#include <vector>

class OrbitWeapon : public Weapon {
public:
  GameObject *owner;
  int count;
  float orbitRadius;
  float orbitSpeedDegrees;
  float orbitRotationDegrees;
  float colliderRadius;
  float knockbackForce;
  float hitCooldown;
  SpritePreset orbitSpritePreset;
  std::vector<GameObject *> orbitObjects;

  OrbitWeapon(GameObject *owner, float damage, int count, float orbitRadius,
              float orbitSpeedDegrees, float startAngleDegrees,
              float colliderRadius, float knockbackForce, float hitCooldown,
              const SpritePreset &orbitSpritePreset)
      : Weapon(damage), owner(owner), count(count), orbitRadius(orbitRadius),
        orbitSpeedDegrees(orbitSpeedDegrees),
        orbitRotationDegrees(startAngleDegrees), colliderRadius(colliderRadius),
        knockbackForce(knockbackForce), hitCooldown(hitCooldown),
        orbitSpritePreset(orbitSpritePreset) {}

  void Start() override;
  void Update(float dt) override;
  void Activate() override {}
  void OnDestroy() override;

private:
  void SyncOrbitObjects();
  void UpdateOrbitTransforms();
};
