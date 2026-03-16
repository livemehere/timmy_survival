#pragma once

#include "SpritePreset.hpp"
#include <string>

struct EnemyDefinition {
  std::string name;
  float maxHp;
  float damage;
  float speed;
  float attackRange;
  float attackCooldown;
  float bounceScale;
  float squashScale;
  float colliderRadius;
  SpritePreset spritePreset;
};
