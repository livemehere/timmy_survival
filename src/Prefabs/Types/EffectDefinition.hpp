#pragma once

#include <string>

enum class EffectType {
  DAMAGE_BURST,
  DAMAGE_ZONE,
};

struct EffectDefinition {
  std::string name;
  EffectType type;
  float damage = 0.0f;
  float width = 0.0f;
  float height = 0.0f;
  float hitDelay = 0.0f;
  float lifetime = 0.0f;
  float knockbackForce = 0.0f;
  float hitCooldown = -1.0f;
  float tickInterval = 0.0f;
};
