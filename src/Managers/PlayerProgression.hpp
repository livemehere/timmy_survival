#pragma once

struct PlayerProgression {
  int energyBallCount = 1;
  int energyBallAttackSpeedLevel = 0;
  int energyBallDamageLevel = 0;
  int energyBallRangeLevel = 0;
  int energyBallProjectilePierceLevel = 0;
  int energyBallProjectileKnockbackLevel = 0;

  bool orbitUnlocked = false;
  int orbitCount = 1;
  int orbitSpeedLevel = 0;
  int orbitDamageLevel = 0;

  bool homingMissileUnlocked = false;
  int homingMissileCount = 1;
  int homingMissileDamageLevel = 0;
  int homingMissileSpeedLevel = 0;

  bool damageBurstUnlocked = false;
  int damageBurstDamageLevel = 0;
  int damageBurstDelayLevel = 0;  // Renamed from cooldownLevel
  int damageBurstSizeLevel = 0;
  int damageBurstKeepingCount = 1;  // Default: 1 charge

  bool damageZoneUnlocked = false;
  int damageZoneDamageLevel = 0;
  int damageZoneDurationLevel = 0;
  int damageZoneTickLevel = 0;
  int damageZoneSizeLevel = 0;
  int damageZoneKeepingCount = 1;  // Default: 1 charge

  int moveSpeedLevel = 0;
  int pickupRadiusLevel = 0;
};
