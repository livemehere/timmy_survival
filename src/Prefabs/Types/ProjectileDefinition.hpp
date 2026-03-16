#pragma once

#include "SpritePreset.hpp"
#include <string>

struct ProjectileDefinition {
  std::string name;
  float damage;
  float lifetime;
  float speed;
  float colliderRadius;
  int pierce;
  float knockbackForce;
  SpritePreset spritePreset;
  bool rotateToVelocity = false;
  float rotationOffset = 0.0f;
};
