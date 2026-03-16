#pragma once

#include "../Types/EffectDefinition.hpp"

namespace EffectDefinitions {
inline const EffectDefinition DAMAGE_BURST = {
    .name = "effect_damageBurst",
    .type = EffectType::DAMAGE_BURST,
    .damage = 3.0f,
    .width = 126.0f,
    .height = 56.0f,
    .hitDelay = 0.2f,
    .lifetime = 0.45f,
    .knockbackForce = 320.0f,
    .hitCooldown = 0.0f,
};

inline const EffectDefinition DAMAGE_ZONE = {
    .name = "effect_damageZone",
    .type = EffectType::DAMAGE_ZONE,
    .damage = 1.0f,
    .width = 128.0f,
    .height = 68.0f,
    .hitDelay = 0.0f,
    .lifetime = 3.0f,
    .knockbackForce = 30.0f,
    .hitCooldown = 0.0f,
    .tickInterval = 0.35f,
};
} // namespace EffectDefinitions
