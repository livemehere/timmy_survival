#pragma once

#include "raylib.h"

#include "GameObject.hpp"
#include "World.hpp"

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position);
GameObject *CreateKnight(World &world, Vector2 position, GameObject *target);
GameObject *CreateCoin(World &world, Vector2 position, GameObject *target);
} // namespace Prefabs
