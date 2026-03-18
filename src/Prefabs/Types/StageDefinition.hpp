#pragma once

#include "EnemyDefinition.hpp"
#include <string>
#include <vector>

struct StageEnemyEntry {
  const EnemyDefinition *enemy = nullptr;
  int weight = 1;
};

struct StageDefinition {
  std::string name;
  float duration = 0.0f;
  int totalSpawnMin = 0;
  int totalSpawnMax = 0;
  float spawnIntervalJitter = 0.3f;
  float spawnRadiusMin = 300.0f;
  float spawnRadiusMax = 700.0f;
  std::vector<StageEnemyEntry> enemies;

  // Infinite stage scaling parameters
  float healthScaling = 1.0f;        // Next stage health multiplier
  float spawnCountScaling = 1.0f;    // Next stage spawn count multiplier
  float spawnSpeedScaling = 1.0f;    // Next stage spawn speed multiplier (lower = faster)
  bool isRepeatable = true;          // Can this stage be repeated
  int maxBossCount = 0;              // Maximum number of bosses on screen (0 = unlimited)
};
