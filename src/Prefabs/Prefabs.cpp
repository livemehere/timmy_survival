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
#include "../Core/GameObject.hpp"
#include "../Managers/GameManager.hpp"
#include "Animation.hpp"
#include <string>

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position) {
  // Core
  GameObject *player = world.CreateObject("player1");
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
  auto sprite = player->AddComponent<SpriteRenderer>();
  sprite->AddAnimation(Animation::PLAYER_IDLE);
  sprite->AddAnimation(Animation::PLAYER_WALK);
  sprite->anchorRatio = {0.5f, 0.75f};

  // Weapon Core
  auto weapon = world.CreateObject("EnergyBall");
  weapon->layer = Layer::WEAPON;

  // Weapon sprite
  weapon->AddComponent<SpriteRenderer>(Animation::ENERGY_BALL,
                                       Vector2{0.4f, 0.4f});

  // Weapon behavior
  weapon->AddComponent<Follow>(player, Vector2{-10.0f, -10.0f}, 10.0f);
  weapon->AddComponent<FireWeapon>(1.0f, 0.5f, 300.0f, 2.0f, 2.0f, 200.0f,
                                   Animation::ENERGY_BALL, Vector2{0.4f, 0.4f});

  return player;
}

GameObject *CreateKnight(World &world, Vector2 position, GameObject *target) {
  GameObject *enemy = world.CreateObject("enemy");
  enemy->layer = Layer::ENEMY;
  enemy->position = position;

  enemy->AddComponent<Velocity>(Vector2{0.0f, 0.0f}, 15.0f);
  enemy->AddComponent<CircleCollider>(8.0f);
  enemy->AddComponent<EnemyAI>(target, 35.0f);

  auto health = enemy->AddComponent<Health>(3.0f);
  health->onDeath = [enemy, &world]() {
    // knight->world->gameManager->AddShock(knight->position);
    CreateCoin(world, enemy->position);
  };

  health->onDamage = [enemy, &world](float damage) {
    auto obj = world.CreateObject("text");
    obj->position = enemy->position;
    obj->AddComponent<TextRenderer>(std::to_string((int)damage), RED, 20, 1.0f,
                                    true, true, BLACK, 2.0f, true);
    obj->AddComponent<Velocity>(Vector2{0.0f, -100.0f}, 3.0f);
    obj->AddComponent<Lifetime>(1.0f);
  };

  // Body sprite
  auto sprite = enemy->AddComponent<SpriteRenderer>();
  sprite->AddAnimation(Animation::KNIGHT_IDLE);
  sprite->AddAnimation(Animation::KNIGHT_WALK);
  sprite->anchorRatio = {0.5f, 0.75f};

  return enemy;
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

  coin->AddComponent<SpriteRenderer>(Animation::COIN_IDLE);

  return coin;
}
} // namespace Prefabs
