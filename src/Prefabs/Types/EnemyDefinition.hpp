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
  float colliderRadius;
  float scale = 1.0f;       // Visual scale multiplier
  bool isBoss = false;       // Boss enemy flag

  // Charging attack system
  float chargeDuration = 1.0f;     // Time to charge before dash
  float dashDuration = 0.3f;       // Dash attack duration
  float dashSpeedMultiplier = 3.0f; // Speed multiplier during dash

  SpritePreset spritePreset;
};
