#pragma once

#include "../Types/ProjectileDefinition.hpp"
#include "Sprites.hpp"

namespace ProjectileDefinitions {
inline const ProjectileDefinition KNIFE = {
    .name = "projectile_knife",
    .damage = 1.7f,  // 1.0 -> 1.7 (balance adjustment)
    .lifetime = 2.0f,
    .speed = 300.0f,
    .colliderRadius = 2.0f,
    .pierce = 1,
    .knockbackForce = 150.0f,
    .spritePreset =
        {
            .clips = {SpriteClips::KNIFE},
            .defaultClipName = "Knife",
            .scale = {0.7f, 0.7f},
            .anchorRatio = {0.5f, 0.5f},
            .offset = {0.0f, 0.0f},
            .tint = WHITE,
        },
    .rotateToVelocity = true,
    .rotationOffset = 90.0f,
    .isHoming = false,
};

inline const ProjectileDefinition HOMING_MISSILE = {
    .name = "projectile_homingMissile",
    .damage = 1.35f,
    .lifetime = 2.8f,
    .speed = 170.0f,
    .colliderRadius = 4.0f,
    .pierce = 1,
    .knockbackForce = 55.0f,  // Reduced knockback
    .spritePreset =
        {
            .clips = {SpriteClips::KNIFE},
            .defaultClipName = "Knife",
            .scale = {0.8f, 0.8f},
            .anchorRatio = {0.5f, 0.5f},
            .offset = {0.0f, 0.0f},
            .tint = PURPLE,  // Use purple for distinction
        },
    .rotateToVelocity = true,
    .rotationOffset = 90.0f,
    .isHoming = true,
    .homingTurnRate = 4.2f,
};
} // namespace ProjectileDefinitions
