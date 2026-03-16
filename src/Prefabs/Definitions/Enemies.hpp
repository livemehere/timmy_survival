#pragma once

#include "Sprites.hpp"
#include "../Types/EnemyDefinition.hpp"

namespace EnemyDefinitions {
inline const EnemyDefinition KNIGHT = {
    "Knight",
    3.0f,
    1.0f,
    35.0f,
    20.0f,
    1.0f,
    0.15f,
    0.15f,
    8.0f,
    {{SpriteClips::KNIGHT_IDLE, SpriteClips::KNIGHT_WALK},
     "Idle",
     {1.0f, 1.0f},
     {0.5f, 0.75f},
     {0.0f, 0.0f},
     WHITE},
};
} // namespace EnemyDefinitions
