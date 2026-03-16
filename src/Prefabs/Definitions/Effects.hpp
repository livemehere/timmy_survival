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
} // namespace EffectDefinitions
