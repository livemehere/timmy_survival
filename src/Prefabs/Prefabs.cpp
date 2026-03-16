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
#include "Animations.hpp"
#include <string>

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position) {
  GameObject *player = world.CreateObject("player1");
  player->position = position;
  player->AddComponent<CircleCollider>(8.0f);
  player->AddComponent<PlayerController>(100.0f);

  // Body sprite
  auto sprite = player->AddComponent<SpriteRenderer>();
  sprite->AddAnimation(Animation::PLAYER_IDLE);
  sprite->AddAnimation(Animation::PLAYER_WALK);
  sprite->anchorRatio = {0.5f, 0.75f};

  // Weapon
  auto weapon = world.CreateObject("KnifeWeapon");
  weapon->layer = Layer::WEAPON;
  weapon->AddComponent<Follow>(player, Vector2{10.0f, -10.0f});

  auto fireWeapon =
      weapon->AddComponent<FireWeapon>(1.0f, 0.1f, 300.0f, 2.0f, 2.0f, 200.0f);
  fireWeapon->projectileAnimConfig = Animation::ENERGY_BALL;
  fireWeapon->projectileAnimScale = {0.4f, 0.4f};

  auto ws = weapon->AddComponent<SpriteRenderer>();
  ws->AddAnimation(Animation::ENERGY_BALL);
  ws->scale = {0.4f, 0.4f};

  // item magnet
  auto magnetCollider = player->AddComponent<CircleCollider>(50.0f);
  magnetCollider->isTrigger = true;
  magnetCollider->onTriggerEnter.AddListener([player](Collider *other) {
    if (other->gameObject->layer == Layer::ITEM) {
      auto magnet = other->gameObject->GetComponent<Magnet>();
      if (magnet) {
        magnet->targetPos = &(player->position);
      }
    }
  });

  return player;
}

GameObject *CreateKnight(World &world, Vector2 position, GameObject *target) {
  GameObject *knight = world.CreateObject("knight");
  knight->layer = Layer::ENEMY;
  knight->position = position;

  knight->AddComponent<Velocity>(Vector2{0.0f, 0.0f}, 15.0f);
  knight->AddComponent<CircleCollider>(8.0f);
  knight->AddComponent<EnemyAI>(target, 35.0f);

  auto health = knight->AddComponent<Health>(3.0f);
  health->onDeath = [knight, &world]() {
    // knight->world->gameManager->AddShock(knight->position);
    CreateCoin(world, knight->position);
  };

  health->onDamage = [knight, &world](float damage) {
    auto obj = world.CreateObject("text");
    obj->position = knight->position;
    obj->AddComponent<TextRenderer>(std::to_string((int)damage), RED, 20, 1.0f,
                                    true, true, BLACK, 2.0f, true);
    obj->AddComponent<Velocity>(Vector2{0.0f, -100.0f}, 3.0f);
    obj->AddComponent<Lifetime>(1.0f);
  };

  // Body sprite
  auto sprite = knight->AddComponent<SpriteRenderer>();
  sprite->AddAnimation(Animation::KNIGHT_IDLE);
  sprite->AddAnimation(Animation::KNIGHT_WALK);
  sprite->anchorRatio = {0.5f, 0.75f};

  return knight;
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

  auto sprite = coin->AddComponent<SpriteRenderer>();
  sprite->AddAnimation(Animation::COIN_IDLE);

  return coin;
}
} // namespace Prefabs
