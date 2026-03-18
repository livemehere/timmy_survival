#pragma once

#include "../Types/StageDefinition.hpp"
#include "Enemies.hpp"
#include <string>
#include <vector>

namespace StageDefinitions {
inline const std::vector<StageDefinition> ALL = {
    {
        .name = "Stage 1",
        .duration = 30.0f,
        .totalSpawnMin = 40,
        .totalSpawnMax = 54,
        .spawnIntervalJitter = 0.16f,
        .spawnRadiusMin = 360.0f,
        .spawnRadiusMax = 760.0f,
        .enemies = {{.enemy = &EnemyDefinitions::KNIGHT, .weight = 100}},
        .healthScaling = 1.22f,      // Next stage: +22% HP
        .spawnCountScaling = 1.16f,  // Next stage: +16% spawn count
        .spawnSpeedScaling = 0.92f,  // Next stage: 8% faster spawn
        .isRepeatable = true,
        .maxBossCount = 0,           // No boss limit for stage 1
    },
    {
        .name = "Stage 2",
        .duration = 60.0f,
        .totalSpawnMin = 66,
        .totalSpawnMax = 88,
        .spawnIntervalJitter = 0.18f,
        .spawnRadiusMin = 340.0f,
        .spawnRadiusMax = 780.0f,
        .enemies = {{.enemy = &EnemyDefinitions::KNIGHT, .weight = 45},
                    {.enemy = &EnemyDefinitions::WIZARD_FEMALE, .weight = 30},
                    {.enemy = &EnemyDefinitions::WIZARD_MALE, .weight = 25}},
        .healthScaling = 1.28f,      // Next stage: +28% HP
        .spawnCountScaling = 1.2f,   // Next stage: +20% spawn count
        .spawnSpeedScaling = 0.86f,  // Next stage: 14% faster spawn
        .isRepeatable = true,
        .maxBossCount = 0,           // No boss limit for stage 2
    },
    {
        .name = "Stage 3",
        .duration = 60.0f,
        .totalSpawnMin = 105,
        .totalSpawnMax = 140,
        .spawnIntervalJitter = 0.25f,
        .spawnRadiusMin = 320.0f,
        .spawnRadiusMax = 720.0f,
        .enemies = {{.enemy = &EnemyDefinitions::KNIGHT, .weight = 22},
                    {.enemy = &EnemyDefinitions::WIZARD_FEMALE, .weight = 15},
                    {.enemy = &EnemyDefinitions::WIZARD_MALE, .weight = 15},
                    {.enemy = &EnemyDefinitions::DINOSAUR, .weight = 20},
                    {.enemy = &EnemyDefinitions::BOSS_BRUTE, .weight = 2},
                    {.enemy = &EnemyDefinitions::GOLEM, .weight = 2},
                    {.enemy = &EnemyDefinitions::VENOM, .weight = 2}},
        .healthScaling = 1.36f,      // Next stage: +36% HP
        .spawnCountScaling = 1.26f,  // Next stage: +26% spawn count
        .spawnSpeedScaling = 0.8f,   // Next stage: 20% faster spawn
        .isRepeatable = true,        // Repeat Stage 3 pattern infinitely
        .maxBossCount = 3,           // Max 3 bosses on screen
    },
};

// Dynamic stage generation function for infinite mode
inline StageDefinition CreateScaledStage(int baseStageIndex, int repeatCount,
                                         float accumulatedHealthScale,
                                         float accumulatedSpawnScale,
                                         float accumulatedSpeedScale) {
  if (baseStageIndex < 0 || baseStageIndex >= static_cast<int>(ALL.size())) {
    baseStageIndex = static_cast<int>(ALL.size()) - 1;
  }

  const StageDefinition &base = ALL[baseStageIndex];
  StageDefinition scaled = base;

  // Update stage name
  scaled.name = "Stage " + std::to_string(baseStageIndex + 1) + " (" +
                 std::to_string(repeatCount + 1) + "x)";

  // Health scaling is applied in CreateEnemy
  // Here we just store the accumulated scale value
  scaled.healthScaling = accumulatedHealthScale;

  // Spawn count scaling
  scaled.totalSpawnMin = static_cast<int>(base.totalSpawnMin * accumulatedSpawnScale);
  scaled.totalSpawnMax = static_cast<int>(base.totalSpawnMax * accumulatedSpawnScale);

  // Spawn speed scaling (reduce duration = faster spawn)
  scaled.duration = base.duration * accumulatedSpeedScale;

  // Increase spawn radius slightly (difficulty ramp)
  scaled.spawnRadiusMin = std::max(260.0f, base.spawnRadiusMin - (repeatCount * 10.0f));
  scaled.spawnRadiusMax = base.spawnRadiusMax + (repeatCount * 12.0f);

  return scaled;
}
} // namespace StageDefinitions
