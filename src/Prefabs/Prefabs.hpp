#pragma once

#include "raylib.h"

#include "Types/EnemyDefinition.hpp"
#include "Types/EffectDefinition.hpp"
#include "Types/ProjectileDefinition.hpp"
#include "Types/WeaponDefinition.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/World.hpp"

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position);
GameObject *CreateEnemy(World &world, Vector2 position, GameObject *target,
                        const EnemyDefinition &definition, float healthScale = 1.0f);
GameObject *CreateEffect(World &world, Vector2 position,
                         const EffectDefinition &definition);
GameObject *CreateWeapon(World &world, GameObject *owner,
                         const WeaponDefinition &definition);
GameObject *CreateProjectile(World &world, Vector2 position, Vector2 velocity,
                             const ProjectileDefinition &definition,
                             float damageOverride = -1.0f);
GameObject *CreateCoin(World &world, Vector2 position);
} // namespace Prefabs
