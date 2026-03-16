#pragma once

#include "raylib.h"

#include "Types/EnemyDefinition.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/World.hpp"

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position);
GameObject *CreateEnemy(World &world, Vector2 position, GameObject *target,
                        const EnemyDefinition &definition);
GameObject *CreateCoin(World &world, Vector2 position);
} // namespace Prefabs
