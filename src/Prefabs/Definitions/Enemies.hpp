#pragma once

#include "../Types/EnemyDefinition.hpp"
#include "Sprites.hpp"

namespace EnemyDefinitions {
inline const EnemyDefinition KNIGHT = {
    .name = "enemy_knight",
    .maxHp = 9.0f,
    .damage = 8.0f,
    .speed = 68.0f,
    .attackRange = 30.0f,
    .attackCooldown = 2.4f,
    .bounceScale = 0.15f,
    .colliderRadius = 8.0f,
    .chargeDuration = 1.0f,
    .dashDuration = 0.3f,
    .dashSpeedMultiplier = 3.0f,
    .spritePreset = SpritePresets::KNIGHT,
};

inline const EnemyDefinition WIZARD_FEMALE = {
    .name = "enemy_wizardFemale",
    .maxHp = 8.0f,
    .damage = 7.0f,
    .speed = 62.0f,
    .attackRange = 32.0f,
    .attackCooldown = 2.4f,
    .bounceScale = 0.15f,
    .colliderRadius = 8.0f,
    .chargeDuration = 1.0f,
    .dashDuration = 0.3f,
    .dashSpeedMultiplier = 3.0f,
    .spritePreset = SpritePresets::WIZARD_FEMALE,
};

inline const EnemyDefinition WIZARD_MALE = {
    .name = "enemy_wizardMale",
    .maxHp = 8.0f,
    .damage = 7.0f,
    .speed = 64.0f,
    .attackRange = 32.0f,
    .attackCooldown = 2.4f,
    .bounceScale = 0.15f,
    .colliderRadius = 8.0f,
    .chargeDuration = 1.0f,
    .dashDuration = 0.3f,
    .dashSpeedMultiplier = 3.0f,
    .spritePreset = SpritePresets::WIZARD_MALE,
};

inline const EnemyDefinition DINOSAUR = {
    .name = "enemy_dinosaur",
    .maxHp = 14.0f,
    .damage = 10.0f,
    .speed = 76.0f,
    .attackRange = 34.0f,
    .attackCooldown = 2.6f,
    .bounceScale = 0.18f,
    .colliderRadius = 10.0f,
    .chargeDuration = 1.0f,
    .dashDuration = 0.3f,
    .dashSpeedMultiplier = 3.0f,
    .spritePreset = SpritePresets::DINOSAUR,
};

inline const EnemyDefinition BOSS_BRUTE = {
    .name = "enemy_bossBrute",
    .maxHp = 120.0f,
    .damage = 12.0f,
    .speed = 52.0f,
    .attackRange = 42.0f,
    .attackCooldown = 3.0f,
    .bounceScale = 0.2f,
    .colliderRadius = 14.0f,
    .scale = 2.0f,
    .isBoss = true,
    .chargeDuration = 1.2f,
    .dashDuration = 0.4f,
    .dashSpeedMultiplier = 3.0f,
    .spritePreset = SpritePresets::BOSS_BRUTE,
};

inline const EnemyDefinition GOLEM = {
    .name = "enemy_golem",
    .maxHp = 150.0f,
    .damage = 14.0f,
    .speed = 48.0f,
    .attackRange = 44.0f,
    .attackCooldown = 3.2f,
    .bounceScale = 0.22f,
    .colliderRadius = 15.0f,
    .scale = 2.0f,
    .isBoss = true,
    .chargeDuration = 1.2f,
    .dashDuration = 0.4f,
    .dashSpeedMultiplier = 3.0f,
    .spritePreset = SpritePresets::GOLEM,
};

inline const EnemyDefinition VENOM = {
    .name = "enemy_venom",
    .maxHp = 135.0f,
    .damage = 13.0f,
    .speed = 64.0f,
    .attackRange = 42.0f,
    .attackCooldown = 2.8f,
    .bounceScale = 0.2f,
    .colliderRadius = 14.0f,
    .scale = 2.0f,
    .isBoss = true,
    .chargeDuration = 1.2f,
    .dashDuration = 0.4f,
    .dashSpeedMultiplier = 3.0f,
    .spritePreset = SpritePresets::VENOM,
};
} // namespace EnemyDefinitions
