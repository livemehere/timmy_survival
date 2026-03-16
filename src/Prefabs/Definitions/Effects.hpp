#pragma once

#include "../Types/EffectDefinition.hpp"

namespace EffectDefinitions {
inline const EffectDefinition LIGHTNING_STRIKE = {
    .name = "effect_lightningStrike",
    .type = EffectType::LIGHTNING_STRIKE,
    .damage = 3.0f,
    .radius = 28.0f,
    .hitDelay = 0.2f,
    .lifetime = 0.45f,
    .knockbackForce = 320.0f,
    .hitCooldown = 0.0f,
};

inline const EffectDefinition DAMAGE_ZONE = {
    .name = "effect_damageZone",
    .type = EffectType::DAMAGE_ZONE,
    .damage = 1.0f,
    .radius = 34.0f,
    .hitDelay = 0.0f,
    .lifetime = 3.0f,
    .knockbackForce = 30.0f,
    .hitCooldown = 0.0f,
    .tickInterval = 0.35f,
};
} // namespace EffectDefinitions
