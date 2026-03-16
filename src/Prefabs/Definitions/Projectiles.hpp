#pragma once

#include "../Types/ProjectileDefinition.hpp"
#include "Sprites.hpp"

namespace ProjectileDefinitions {
inline const ProjectileDefinition KNIFE = {
    .name = "projectile_knife",
    .damage = 1.0f,
    .lifetime = 2.0f,
    .speed = 300.0f,
    .colliderRadius = 2.0f,
    .pierce = 1,
    .knockbackForce = 150.0f,
    .spritePreset =
        {
            .clips = {SpriteClips::KNIFE},
            .defaultClipName = "Knife",
            .scale = {0.4f, 0.4f},
            .anchorRatio = {0.5f, 0.5f},
            .offset = {0.0f, 0.0f},
            .tint = WHITE,
        },
    .rotateToVelocity = true,
    .rotationOffset = 90.0f,
};
} // namespace ProjectileDefinitions
