#pragma once

#include "ProjectileDefinition.hpp"
#include "SpritePreset.hpp"
#include <string>

struct WeaponDefinition {
  std::string name;
  float cooldown;
  float range;
  Vector2 followOffset = {0.0f, 0.0f};
  float followSpeed = 0.0f;
  SpritePreset spritePreset;
  ProjectileDefinition projectileDefinition;
};
