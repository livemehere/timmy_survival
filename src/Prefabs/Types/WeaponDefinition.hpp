#pragma once

#include "ProjectileDefinition.hpp"
#include "SpritePreset.hpp"
#include <string>

enum class WeaponType {
  FIRE,
  ORBIT,
};

struct WeaponDefinition {
  WeaponType type = WeaponType::FIRE;
  std::string name;
  float damage = -1.0f;
  float cooldown;
  float range;
  Vector2 followOffset = {0.0f, 0.0f};
  float followSpeed = 0.0f;
  float orbitRadius = 0.0f;
  float orbitSpeedDegrees = 0.0f;
  float startAngleDegrees = 0.0f;
  float colliderRadius = 0.0f;
  SpritePreset spritePreset;
  ProjectileDefinition projectileDefinition;
};
