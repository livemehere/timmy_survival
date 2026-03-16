#pragma once

#include "../Types/EnemyDefinition.hpp"
#include "Sprites.hpp"

namespace EnemyDefinitions {
inline const EnemyDefinition KNIGHT = {
    .name = "enemy_knight",
    .maxHp = 3.0f,
    .damage = 1.0f,
    .speed = 35.0f,
    .attackRange = 20.0f,
    .attackCooldown = 1.0f,
    .bounceScale = 0.15f,
    .colliderRadius = 8.0f,
    .spritePreset =
        {
            .clips = {SpriteClips::KNIGHT_IDLE, SpriteClips::KNIGHT_WALK},
            .defaultClipName = "Idle",
            .scale = {1.0f, 1.0f},
            .anchorRatio = {0.5f, 0.75f},
            .offset = {0.0f, 0.0f},
            .tint = WHITE,
        },
};
} // namespace EnemyDefinitions
