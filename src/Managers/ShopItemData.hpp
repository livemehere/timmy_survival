#pragma once

#include "PlayerProgression.hpp"
#include "ShopTypes.hpp"
#include <unordered_map>

// All shop item data table (data-driven design)
// Add new items here only!
inline const std::unordered_map<ShopItemId, ShopItemConfig> kShopItemData = {
    // Energy Ball
    {ShopItemId::ADD_ENERGY_BALL,
     {ShopItemId::ADD_ENERGY_BALL, ShopCategory::WEAPON, "Energy Ball +1",
      "Add one more energy ball. Max 2.", 12, 16,
      [](const PlayerProgression &p) { return p.energyBallCount < 2; }}},

    {ShopItemId::ENERGY_BALL_ATTACK_SPEED,
     {ShopItemId::ENERGY_BALL_ATTACK_SPEED, ShopCategory::WEAPON,
      "Energy Ball Haste", "All energy balls attack 10% faster.", 8, 10,
      [](const PlayerProgression &p) { return p.energyBallCount > 0; }}},

    {ShopItemId::ENERGY_BALL_DAMAGE_UP,
     {ShopItemId::ENERGY_BALL_DAMAGE_UP, ShopCategory::WEAPON,
      "Energy Ball Power", "All energy balls deal more damage.", 9, 10,
      [](const PlayerProgression &p) { return p.energyBallCount > 0; }}},

    {ShopItemId::ENERGY_BALL_RANGE_UP,
     {ShopItemId::ENERGY_BALL_RANGE_UP, ShopCategory::WEAPON,
      "Energy Ball Range Up", "Energy balls travel farther.", 8, 10,
      [](const PlayerProgression &p) { return p.energyBallCount > 0; }}},

    {ShopItemId::ENERGY_BALL_PIERCE_UP,
     {ShopItemId::ENERGY_BALL_PIERCE_UP, ShopCategory::WEAPON,
      "Energy Ball Pierce Up", "Energy balls can pierce through more enemies.",
      10, 12,
      [](const PlayerProgression &p) { return p.energyBallCount > 0; }}},

    {ShopItemId::ENERGY_BALL_KNOCKBACK_UP,
     {ShopItemId::ENERGY_BALL_KNOCKBACK_UP, ShopCategory::WEAPON,
      "Energy Ball Knockback Up", "Energy balls knock enemies back further.",
      8, 10,
      [](const PlayerProgression &p) { return p.energyBallCount > 0; }}},

    // Orbit Weapon
    {ShopItemId::UNLOCK_ORBIT_WEAPON,
     {ShopItemId::UNLOCK_ORBIT_WEAPON, ShopCategory::WEAPON,
      "Unlock Orbit Blade", "Gain a rotating blade weapon.", 14, 18,
      [](const PlayerProgression &p) { return !p.orbitUnlocked; }}},

    {ShopItemId::ADD_ORBIT_BLADE,
     {ShopItemId::ADD_ORBIT_BLADE, ShopCategory::WEAPON, "Orbit Blade +1",
      "Add one orbit blade. Max 5.", 10, 16,
      [](const PlayerProgression &p) {
        return p.orbitUnlocked && p.orbitCount < 5;
      }}},

    {ShopItemId::ORBIT_SPEED_UP,
     {ShopItemId::ORBIT_SPEED_UP, ShopCategory::WEAPON, "Orbit Speed +10%",
      "Orbit blades spin faster.", 8, 10,
      [](const PlayerProgression &p) { return p.orbitUnlocked; }}},

    {ShopItemId::ORBIT_DAMAGE_UP,
     {ShopItemId::ORBIT_DAMAGE_UP, ShopCategory::WEAPON, "Orbit Power",
      "Orbit blades deal more damage.", 9, 10,
      [](const PlayerProgression &p) { return p.orbitUnlocked; }}},

    // Homing Missile
    {ShopItemId::UNLOCK_HOMING_MISSILE,
     {ShopItemId::UNLOCK_HOMING_MISSILE, ShopCategory::WEAPON,
      "Unlock Homing Missile", "Gain a homing missile weapon.", 16, 18,
      [](const PlayerProgression &p) { return !p.homingMissileUnlocked; }}},

    {ShopItemId::ADD_HOMING_MISSILE,
     {ShopItemId::ADD_HOMING_MISSILE, ShopCategory::WEAPON, "Homing Missile +1",
      "Add one homing missile. Max 3.", 12, 16,
      [](const PlayerProgression &p) {
        return p.homingMissileUnlocked && p.homingMissileCount < 3;
      }}},

    {ShopItemId::HOMING_MISSILE_DAMAGE_UP,
     {ShopItemId::HOMING_MISSILE_DAMAGE_UP, ShopCategory::WEAPON,
      "Homing Missile Power", "Homing missiles deal more damage.", 10, 10,
      [](const PlayerProgression &p) { return p.homingMissileUnlocked; }}},

    {ShopItemId::HOMING_MISSILE_SPEED_UP,
     {ShopItemId::HOMING_MISSILE_SPEED_UP, ShopCategory::WEAPON,
      "Homing Speed Up", "Homing missiles turn faster.", 9, 10,
      [](const PlayerProgression &p) { return p.homingMissileUnlocked; }}},

    // Damage Burst (Skill)
    {ShopItemId::UNLOCK_DAMAGE_BURST,
     {ShopItemId::UNLOCK_DAMAGE_BURST, ShopCategory::SKILL, "Equip Burst Skill",
      "Left click casts a burst strike.", 12, 18,
      [](const PlayerProgression &p) { return !p.damageBurstUnlocked; }}},

    {ShopItemId::DAMAGE_BURST_DAMAGE_UP,
     {ShopItemId::DAMAGE_BURST_DAMAGE_UP, ShopCategory::SKILL,
      "Burst Damage Up", "Burst hits harder.", 9, 10,
      [](const PlayerProgression &p) { return p.damageBurstUnlocked; }}},

    {ShopItemId::DAMAGE_BURST_DELAY_DOWN,
     {ShopItemId::DAMAGE_BURST_DELAY_DOWN, ShopCategory::SKILL,
      "Burst Delay Down", "Burst lands faster.", 9, 10,
      [](const PlayerProgression &p) { return p.damageBurstUnlocked; }}},

    {ShopItemId::DAMAGE_BURST_SIZE_UP,
     {ShopItemId::DAMAGE_BURST_SIZE_UP, ShopCategory::SKILL, "Burst Area Up",
      "Burst area becomes larger.", 9, 10,
      [](const PlayerProgression &p) { return p.damageBurstUnlocked; }}},

    {ShopItemId::DAMAGE_BURST_KEEPING_COUNT_UP,
     {ShopItemId::DAMAGE_BURST_KEEPING_COUNT_UP, ShopCategory::SKILL,
      "Burst Keeping Count +1", "You can keep one more burst strike. Max 3.",
      10, 12,
      [](const PlayerProgression &p) {
        return p.damageBurstUnlocked && p.damageBurstKeepingCount < 3;
      }}},

    {ShopItemId::DAMAGE_ZONE_KEEPING_COUNT_UP,
     {ShopItemId::DAMAGE_ZONE_KEEPING_COUNT_UP, ShopCategory::SKILL,
      "Gas Keeping Count +1", "You can keep one more gas zone. Max 3.", 10, 12,
      [](const PlayerProgression &p) {
        return p.damageZoneUnlocked && p.damageZoneKeepingCount < 3;
      }}},

    // Damage Zone (Skill)
    {ShopItemId::UNLOCK_DAMAGE_ZONE,
     {ShopItemId::UNLOCK_DAMAGE_ZONE, ShopCategory::SKILL, "Equip Poison Gas",
      "Right click casts a damage zone.", 12, 18,
      [](const PlayerProgression &p) { return !p.damageZoneUnlocked; }}},

    {ShopItemId::DAMAGE_ZONE_DAMAGE_UP,
     {ShopItemId::DAMAGE_ZONE_DAMAGE_UP, ShopCategory::SKILL, "Gas Damage Up",
      "Damage zone deals more damage.", 9, 10,
      [](const PlayerProgression &p) { return p.damageZoneUnlocked; }}},

    {ShopItemId::DAMAGE_ZONE_DURATION_UP,
     {ShopItemId::DAMAGE_ZONE_DURATION_UP, ShopCategory::SKILL,
      "Gas Duration Up", "Damage zone lasts longer.", 8, 10,
      [](const PlayerProgression &p) { return p.damageZoneUnlocked; }}},

    {ShopItemId::DAMAGE_ZONE_TICKRATE_UP,
     {ShopItemId::DAMAGE_ZONE_TICKRATE_UP, ShopCategory::SKILL,
      "Gas Tick Rate Up", "Damage zone hits more often.", 10, 10,
      [](const PlayerProgression &p) { return p.damageZoneUnlocked; }}},

    {ShopItemId::DAMAGE_ZONE_SIZE_UP,
     {ShopItemId::DAMAGE_ZONE_SIZE_UP, ShopCategory::SKILL, "Gas Area Up",
      "Damage zone becomes larger.", 9, 10,
      [](const PlayerProgression &p) { return p.damageZoneUnlocked; }}},

    // Stats
    {ShopItemId::MOVE_SPEED_UP,
     {ShopItemId::MOVE_SPEED_UP, ShopCategory::STAT, "Move Speed Up",
      "Move 8% faster.", 8, 12,
      [](const PlayerProgression &p) { return true; }}},

    {ShopItemId::PICKUP_RADIUS_UP,
     {ShopItemId::PICKUP_RADIUS_UP, ShopCategory::STAT, "Pickup Radius Up",
      "Increase coin pickup range.", 6, 12,
      [](const PlayerProgression &p) { return true; }}},

    // Health Recovery
    {ShopItemId::HEALTH_RECOVERY_SMALL,
     {ShopItemId::HEALTH_RECOVERY_SMALL, ShopCategory::STAT, "Small Heal",
      "Recover 25 HP.", 8, 15,
      [](const PlayerProgression &p) { return true; }}},

    {ShopItemId::HEALTH_RECOVERY_LARGE,
     {ShopItemId::HEALTH_RECOVERY_LARGE, ShopCategory::STAT, "Large Heal",
      "Recover 50 HP.", 15, 12,
      [](const PlayerProgression &p) { return true; }}},
};
