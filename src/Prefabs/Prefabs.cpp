#include "Prefabs.hpp"
#include "../Components/Colliders/CircleCollider.hpp"
#include "../Components/Effects/DamageBurst.hpp"
#include "../Components/Effects/DamageZone.hpp"
#include "../Components/EnemyAI.hpp"
#include "../Components/Health.hpp"
#include "../Components/Lifetime.hpp"
#include "../Components/Movement/Follow.hpp"
#include "../Components/Movement/Magnet.hpp"
#include "../Components/Movement/Velocity.hpp"
#include "../Components/PlayerController.hpp"
#include "../Components/Render/SpriteRenderer.hpp"
#include "../Components/Render/TextRenderer.hpp"
#include "../Components/Weapons/FireWeapon.hpp"
#include "../Components/Weapons/OrbitWeapon.hpp"
#include "../Components/Weapons/Projectile.hpp"
#include "../Core/GameObject.hpp"
#include "../Managers/GameManager.hpp"
#include "../Managers/CameraManager.hpp"
#include "Definitions/Sprites.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <string>

namespace {
SpriteRenderer *ApplySpritePreset(GameObject *gameObject,
                                  const SpritePreset &preset) {
  auto sprite = gameObject->AddComponent<SpriteRenderer>();

  for (const auto &clip : preset.clips) {
    sprite->AddClip(clip);
  }

  sprite->scale = preset.scale;
  sprite->anchorRatio = preset.anchorRatio;
  sprite->offset = preset.offset;
  sprite->tint = preset.tint;

  if (!preset.defaultClipName.empty()) {
    sprite->Play(preset.defaultClipName);
  }

  return sprite;
}

} // namespace

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position) {
  // Core
  GameObject *player = world.CreateObject("player");
  player->layer = Layer::PLAYER;
  player->position = position;
  auto bodyCollider = player->AddComponent<CircleCollider>(8.0f);
  bodyCollider->label = ColliderLabel::PLAYER_BODY;
  player->AddComponent<PlayerController>(140.0f);
  player->AddComponent<Velocity>(Vector2{0.0f, 0.0f}, 9.0f);

  // Add player health
  auto *playerHealth = player->AddComponent<Health>(100.0f);
  playerHealth->invincibilityTime = 0.8f;
  playerHealth->useSharedInvincibility = true;
  playerHealth->onDamage = [player](float damage, GameObject *source) {
    if (!source) {
      return;
    }

    Vector2 pushDir = Vector2Subtract(player->position, source->position);
    if (Vector2LengthSqr(pushDir) <= 0.001f) {
      pushDir = {1.0f, 0.0f};
    } else {
      pushDir = Vector2Normalize(pushDir);
    }

    auto velocity = player->GetComponent<Velocity>();
    if (velocity) {
      float knockbackStrength = 130.0f + damage * 2.0f;
      velocity->Apply(Vector2Scale(pushDir, knockbackStrength));
    }

    CameraManager::Get().Shake(3.5f, 0.12f);
  };
  playerHealth->onDeath = [player]() {
    // Handle game over
    GameManager::Get().gameOver = true;
  };

  // Item Collider
  auto itemCollider = player->AddComponent<CircleCollider>(50.0f);
  itemCollider->isTrigger = true;
  itemCollider->label = ColliderLabel::PLAYER_PICKUP;
  itemCollider->onTriggerEnter.AddListener([player](Collider *other) {
    if (other->gameObject->layer == Layer::ITEM) {
      auto magnet = other->gameObject->GetComponent<Magnet>();
      if (magnet) {
        magnet->targetPos = &(player->position);
        magnet->speed = 220.0f;
        magnet->acceleration = 1000.0f;
        magnet->maxSpeed = 760.0f;
        magnet->onCollect = [other]() {
          other->gameObject->Destroy();
          GameManager::Get().AddCoin(1);
        };
      }
    }
  });

  // Sprite
  ApplySpritePreset(player, SpritePresets::PLAYER);

  return player;
}

GameObject *CreateEffect(World &world, Vector2 position,
                         const EffectDefinition &definition) {
  auto effect = world.CreateObject(definition.name);
  effect->position = position;

  switch (definition.type) {
  case EffectType::DAMAGE_BURST:
    effect->AddComponent<DamageBurst>(
        definition.damage, definition.width, definition.height,
        definition.hitDelay, definition.lifetime, definition.knockbackForce,
        definition.hitCooldown);
    break;
  case EffectType::DAMAGE_ZONE:
    effect->AddComponent<DamageZone>(definition.damage, definition.width,
                                     definition.height, definition.lifetime,
                                     definition.knockbackForce,
                                     definition.tickInterval);
    break;
  }

  return effect;
}

GameObject *CreateEnemy(World &world, Vector2 position, GameObject *target,
                        const EnemyDefinition &definition, float healthScale) {
  GameObject *enemy = world.CreateObject(definition.name);
  enemy->layer = Layer::ENEMY;
  enemy->position = position;

  enemy->AddComponent<Velocity>(Vector2{0.0f, 0.0f}, 15.0f);
  auto collider = enemy->AddComponent<CircleCollider>(definition.colliderRadius);
  collider->label = ColliderLabel::ENEMY_BODY; // Add enemy collider label
  enemy->AddComponent<EnemyAI>(target, definition.speed, definition.damage,
                                definition.attackRange, definition.chargeDuration,
                                definition.dashDuration, definition.dashSpeedMultiplier,
                                definition.attackCooldown);

  float scaledMaxHp = definition.maxHp * healthScale;
  auto health = enemy->AddComponent<Health>(scaledMaxHp);
  health->isBoss = definition.isBoss;  // Set boss flag

  // Boss enemy death effect: coin explosion
  if (definition.isBoss) {
    health->onDeath = [enemy, &world, scaledMaxHp]() {
      int coinCount = std::clamp(static_cast<int>(scaledMaxHp / 3.0f), 18, 42);
      for (int i = 0; i < coinCount; i++) {
        Vector2 spawnOffset = {
            static_cast<float>(GetRandomValue(-12, 12)),
            static_cast<float>(GetRandomValue(-10, 10))};
        Vector2 scatterVel = {
            static_cast<float>(GetRandomValue(-260, 260)),
            static_cast<float>(GetRandomValue(-340, -80)) // Upward bias
        };
        GameObject *coin =
            CreateCoin(world, Vector2Add(enemy->position, spawnOffset));
        auto velocity = coin->GetComponent<Velocity>();
        if (velocity) {
          velocity->Apply(scatterVel);
        }
      }
    };
  } else {
    // Normal enemy death
    health->onDeath = [enemy, &world]() { CreateCoin(world, enemy->position); };
  }

  health->onDamage = [enemy, &world](float damage, GameObject *source) {
    (void)source;
    auto obj = world.CreateObject("text");
    obj->position = enemy->position;
    obj->AddComponent<TextRenderer>(std::to_string((int)damage), RED, 20, 1.0f,
                                    true, true, BLACK, 2.0f, true);
    obj->AddComponent<Velocity>(Vector2{0.0f, -100.0f}, 3.0f);
    obj->AddComponent<Lifetime>(1.0f);
  };

  ApplySpritePreset(enemy, definition.spritePreset);

  // Apply visual scale
  if (definition.scale != 1.0f) {
    auto *sprite = enemy->GetComponent<SpriteRenderer>();
    if (sprite) {
      sprite->scale.x *= definition.scale;
      sprite->scale.y *= definition.scale;
    }
  }

  return enemy;
}

GameObject *CreateWeapon(World &world, GameObject *owner,
                         const WeaponDefinition &definition) {
  auto weapon = world.CreateObject(definition.name);
  weapon->layer = Layer::WEAPON;
  weapon->position = owner->position;

  if (definition.type == WeaponType::FIRE) {
    ApplySpritePreset(weapon, definition.spritePreset);
    float damage = definition.damage >= 0.0f
                       ? definition.damage
                       : definition.projectileDefinition.damage;
    weapon->AddComponent<Follow>(owner, definition.followOffset,
                                 definition.followSpeed);
    weapon->AddComponent<FireWeapon>(damage, definition.cooldown,
                                     definition.range,
                                     definition.projectileDefinition);
    return weapon;
  }

  weapon->AddComponent<OrbitWeapon>(
      owner, definition.damage, definition.count, definition.orbitRadius,
      definition.orbitSpeedDegrees, definition.startAngleDegrees,
      definition.colliderRadius, definition.knockbackForce,
      definition.hitCooldown, definition.spritePreset);

  return weapon;
}

GameObject *CreateProjectile(World &world, Vector2 position, Vector2 velocity,
                             const ProjectileDefinition &definition,
                             float damageOverride) {
  auto projectile = world.CreateObject(definition.name);
  projectile->layer = Layer::PROJECTILE;
  projectile->position = position;

  float damage = damageOverride >= 0.0f ? damageOverride : definition.damage;
  auto *projComponent = projectile->AddComponent<Projectile>(damage, definition.lifetime,
                                       definition.pierce,
                                       definition.knockbackForce);

  // Configure homing behavior
  projComponent->isHoming = definition.isHoming;
  projComponent->rotateToVelocity = definition.rotateToVelocity;
  projComponent->homingTurnRate = definition.homingTurnRate;
  projComponent->rotationOffset = definition.rotationOffset;
  projComponent->speed = definition.speed;

  projectile->AddComponent<Velocity>(velocity, 0.0f);

  auto collider =
      projectile->AddComponent<CircleCollider>(definition.colliderRadius);
  collider->isTrigger = true;

  ApplySpritePreset(projectile, definition.spritePreset);

  return projectile;
}

GameObject *CreateCoin(World &world, Vector2 position) {
  auto coin = world.CreateObject("coin");
  coin->layer = Layer::ITEM;
  coin->position = position;
  auto collider = coin->AddComponent<CircleCollider>(8.0f);
  collider->isTrigger = true;

  coin->AddComponent<Magnet>();
  ApplySpritePreset(coin, SpritePresets::COIN);

  return coin;
}
} // namespace Prefabs
