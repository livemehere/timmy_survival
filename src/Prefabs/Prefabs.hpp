#pragma once

#include "raylib.h"

#include "../Core/GameObject.hpp"
#include "../Core/World.hpp"

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position);
GameObject *CreateKnight(World &world, Vector2 position, GameObject *target);
GameObject *CreateCoin(World &world, Vector2 position, GameObject *target);
} // namespace Prefabs
