#pragma once

#include "../Types/WeaponDefinition.hpp"
#include "Projectiles.hpp"
#include "Sprites.hpp"

namespace WeaponDefinitions {
inline const WeaponDefinition ENERGY_BALL = {
    .name = "weapon_energyBall",
    .cooldown = 0.5f,
    .range = 200.0f,
    .followOffset = {-10.0f, -10.0f},
    .followSpeed = 10.0f,
    .spritePreset = SpritePresets::ENERGY_BALL,
    .projectileDefinition = ProjectileDefinitions::KNIFE,
};
} // namespace WeaponDefinitions
