#include "Prefabs.hpp"
#include "../Components/Colliders/CircleCollider.hpp"
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
#include "../Components/Weapons/Projectile.hpp"
#include "../Core/GameObject.hpp"
#include "../Managers/GameManager.hpp"
#include "Definitions/Sprites.hpp"
#include "Definitions/Weapons.hpp"
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
  player->position = position;
  player->AddComponent<CircleCollider>(8.0f);
  player->AddComponent<PlayerController>(100.0f);

  // Item Collider
  auto itemCollider = player->AddComponent<CircleCollider>(50.0f);
  itemCollider->isTrigger = true;
  itemCollider->onTriggerEnter.AddListener([player](Collider *other) {
    if (other->gameObject->layer == Layer::ITEM) {
      auto magnet = other->gameObject->GetComponent<Magnet>();
      if (magnet) {
        magnet->targetPos = &(player->position);
      }
    }
  });

  // Sprite
  ApplySpritePreset(player, SpritePresets::PLAYER);

  return player;
}

GameObject *CreateEnemy(World &world, Vector2 position, GameObject *target,
                        const EnemyDefinition &definition) {
  GameObject *enemy = world.CreateObject(definition.name);
  enemy->layer = Layer::ENEMY;
  enemy->position = position;

  enemy->AddComponent<Velocity>(Vector2{0.0f, 0.0f}, 15.0f);
  enemy->AddComponent<CircleCollider>(definition.colliderRadius);
  enemy->AddComponent<EnemyAI>(target, definition.speed);

  auto health = enemy->AddComponent<Health>(definition.maxHp);
  health->onDeath = [enemy, &world]() { CreateCoin(world, enemy->position); };

  health->onDamage = [enemy, &world](float damage) {
    auto obj = world.CreateObject("text");
    obj->position = enemy->position;
    obj->AddComponent<TextRenderer>(std::to_string((int)damage), RED, 20, 1.0f,
                                    true, true, BLACK, 2.0f, true);
    obj->AddComponent<Velocity>(Vector2{0.0f, -100.0f}, 3.0f);
    obj->AddComponent<Lifetime>(1.0f);
  };

  ApplySpritePreset(enemy, definition.spritePreset);

  return enemy;
}

GameObject *CreateWeapon(World &world, GameObject *owner,
                         const WeaponDefinition &definition) {
  auto weapon = world.CreateObject(definition.name);
  weapon->layer = Layer::WEAPON;
  weapon->position = owner->position;

  ApplySpritePreset(weapon, definition.spritePreset);
  weapon->AddComponent<Follow>(owner, definition.followOffset,
                               definition.followSpeed);
  weapon->AddComponent<FireWeapon>(definition.projectileDefinition.damage,
                                   definition.cooldown, definition.range,
                                   definition.projectileDefinition);

  return weapon;
}

GameObject *CreateProjectile(World &world, Vector2 position, Vector2 velocity,
                             const ProjectileDefinition &definition,
                             float damageOverride) {
  auto projectile = world.CreateObject(definition.name);
  projectile->layer = Layer::PROJECTILE;
  projectile->position = position;

  float damage = damageOverride >= 0.0f ? damageOverride : definition.damage;
  projectile->AddComponent<Projectile>(damage, definition.lifetime,
                                       definition.pierce,
                                       definition.knockbackForce);
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

  auto magnet = coin->AddComponent<Magnet>();
  magnet->onCollect = [coin]() {
    coin->Destroy();
    GameManager::Get().AddCoin(1);
  };

  ApplySpritePreset(coin, SpritePresets::COIN);

  return coin;
}
} // namespace Prefabs
