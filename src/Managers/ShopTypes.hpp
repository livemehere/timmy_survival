#pragma once

#include <functional>
#include <string>
#include <vector>

// Forward declaration
struct PlayerProgression;

enum class ShopCategory {
  WEAPON,
  STAT,
  SKILL,
};

enum class ShopItemId {
  ADD_ENERGY_BALL,
  ENERGY_BALL_ATTACK_SPEED,
  ENERGY_BALL_DAMAGE_UP,
  ENERGY_BALL_RANGE_UP,
  ENERGY_BALL_PIERCE_UP,
  ENERGY_BALL_KNOCKBACK_UP,
  UNLOCK_ORBIT_WEAPON,
  ADD_ORBIT_BLADE,
  ORBIT_SPEED_UP,
  ORBIT_DAMAGE_UP,
  UNLOCK_HOMING_MISSILE,
  ADD_HOMING_MISSILE,
  HOMING_MISSILE_DAMAGE_UP,
  HOMING_MISSILE_SPEED_UP,
  UNLOCK_DAMAGE_BURST,
  DAMAGE_BURST_DAMAGE_UP,
  DAMAGE_BURST_DELAY_DOWN,
  DAMAGE_BURST_SIZE_UP,
  DAMAGE_BURST_KEEPING_COUNT_UP,
  UNLOCK_DAMAGE_ZONE,
  DAMAGE_ZONE_DAMAGE_UP,
  DAMAGE_ZONE_DURATION_UP,
  DAMAGE_ZONE_TICKRATE_UP,
  DAMAGE_ZONE_SIZE_UP,
  DAMAGE_ZONE_KEEPING_COUNT_UP,
  MOVE_SPEED_UP,
  PICKUP_RADIUS_UP,
  HEALTH_RECOVERY_SMALL,
  HEALTH_RECOVERY_LARGE,
};

// Data-driven design: struct containing all item information
struct ShopItemConfig {
  ShopItemId id;
  ShopCategory category;
  const char *title;
  const char *description;
  int cost;
  int weight; // Probability weight
  std::function<bool(const PlayerProgression &)> canOfferCondition;

  // Cached wrapped text (rendering optimization)
  mutable std::vector<std::string> wrappedDescription;
  mutable bool isWrappedCached = false;
};

struct ShopOffer {
  ShopItemId id;
  ShopCategory category;
  std::string title;
  std::string description;
  int cost = 0;

  // Cached wrapped text
  std::vector<std::string> wrappedDescription;
  bool isWrappedCached = false;
};
