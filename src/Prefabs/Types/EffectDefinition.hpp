#pragma once

#include <string>

enum class EffectType {
  LIGHTNING_STRIKE,
};

struct EffectDefinition {
  std::string name;
  EffectType type;
  float damage = 0.0f;
  float radius = 0.0f;
  float hitDelay = 0.0f;
  float lifetime = 0.0f;
  float knockbackForce = 0.0f;
  float hitCooldown = -1.0f;
};
