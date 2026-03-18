#pragma once

#include "../Types/WeaponDefinition.hpp"
#include "Projectiles.hpp"
#include "Sprites.hpp"

namespace WeaponDefinitions {
inline const WeaponDefinition ENERGY_BALL = {
    .type = WeaponType::FIRE,
    .name = "weapon_energyBall",
    .damage = ProjectileDefinitions::KNIFE.damage,
    .cooldown = 0.5f,
    .range = 200.0f,
    .followOffset = {-10.0f, -10.0f},
    .followSpeed = 10.0f,
    .spritePreset = SpritePresets::ENERGY_BALL,
    .projectileDefinition = ProjectileDefinitions::KNIFE,
};

inline const WeaponDefinition ORBIT_BLADE = {
    .type = WeaponType::ORBIT,
    .name = "weapon_orbitBlade",
    .damage = 2.0f,  // 1.0 -> 2.0 (balance adjustment)
    .knockbackForce = 120.0f,
    .hitCooldown = 0.35f,
    .cooldown = 0.0f,
    .range = 0.0f,
    .followOffset = {0.0f, 0.0f},
    .followSpeed = 0.0f,
    .orbitRadius = 32.0f,
    .orbitSpeedDegrees = 240.0f,
    .startAngleDegrees = 0.0f,
    .count = 1,
    .colliderRadius = 8.0f,
    .spritePreset = SpritePresets::ORBIT_BLADE,
};

inline const WeaponDefinition HOMING_MISSILE = {
    .type = WeaponType::FIRE,
    .name = "weapon_homingMissile",
    .damage = ProjectileDefinitions::HOMING_MISSILE.damage,
    .cooldown = 0.8f,
    .range = 400.0f,
    .followOffset = {15.0f, -10.0f},
    .followSpeed = 10.0f,
    .spritePreset = SpritePresets::ENERGY_BALL,
    .projectileDefinition = ProjectileDefinitions::HOMING_MISSILE,
};
} // namespace WeaponDefinitions
