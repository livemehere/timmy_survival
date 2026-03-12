#include "Prefabs.hpp"
#include "../Managers/GameManager.hpp"
#include "Components/CircleCollider.hpp"
#include "Components/EnemyAI.hpp"
#include "Components/FireWeapon.hpp"
#include "Components/Health.hpp"
#include "Components/Knockback.hpp"
#include "Components/Magnet.hpp"
#include "Components/PlayerController.hpp"
#include "Components/SpriteRenderer.hpp"
#include "GameObject.hpp"
#include <string>

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position) {
  GameObject *player = world.CreateObject("player1");
  player->position = position;
  player->AddComponent<CircleCollider>(8.0f);
  player->AddComponent<PlayerController>(100.0f);

  // Body sprite
  auto sprite = player->AddComponent<SpriteRenderer>();
  sprite->AddAnimation("Idle", "../assets/source.png", 128, 32, 16, 32, 2, 0.5f,
                       true);
  sprite->AddAnimation("Walk", "../assets/source.png", 192, 32, 16, 32, 4,
                       0.15f, true);
  sprite->anchorRatio = {0.5f, 0.75f};
  sprite->Play("Idle");

  // Weapon
  auto fireWeapon =
      player->AddComponent<FireWeapon>(1.0f, 0.1f, 500.0f, 2.0f, 2.0f, 200.0f);

  // item magnet
  auto magnetCollider = player->AddComponent<CircleCollider>(50.0f);
  magnetCollider->isTrigger = true;
  magnetCollider->onTrggerEnter = [player](Collider *other) {
    if (other->gameObject->layer == Layer::ITEM) {
      std::cout << "item enter magnet range" << std::endl;
      auto magnet = other->gameObject->GetComponent<Magnet>();
      if (magnet) {
        magnet->targetPos = &(player->position);
      }
    }
  };

  return player;
}

GameObject *CreateKnight(World &world, Vector2 position, GameObject *target) {
  GameObject *knight = world.CreateObject("knight");
  knight->layer = Layer::ENEMY;
  knight->position = position;

  knight->AddComponent<Knockback>();
  knight->AddComponent<CircleCollider>(8.0f);
  knight->AddComponent<EnemyAI>(target, 35.0f);
  knight->AddComponent<SpriteRenderer>();
  auto health = knight->AddComponent<Health>(3.0f);
  health->onDeath = [knight, &world]() {
    // knight->world->gameManager->AddShock(knight->position);
    CreateCoin(world, knight->position, nullptr);
  };

  health->onDamage = [knight, &world](float damage) {
    auto obj = world.CreateObject("text");
    obj->position = knight->position;
    obj->AddComponent<DamageText>(std::to_string((int)damage), RED, 16);
  };

  // Body sprite
  auto sprite = knight->GetComponent<SpriteRenderer>();
  sprite->AddAnimation("Idle", "../assets/source.png", 128, 64, 16, 32, 2, 0.5f,
                       true);
  sprite->AddAnimation("Walk", "../assets/source.png", 192, 64, 16, 32, 4,
                       0.15f, true);
  sprite->anchorRatio = {0.5f, 0.75f};

  return knight;
}

GameObject *CreateCoin(World &world, Vector2 position, GameObject *target) {
  auto coin = world.CreateObject("coin");
  coin->layer = Layer::ITEM;
  coin->position = position;
  auto collider = coin->AddComponent<CircleCollider>(8.0f);
  collider->isTrigger = true;

  auto magnet = coin->AddComponent<Magnet>();
  magnet->onCollect = [coin, &world]() {
    coin->Destroy();
    world.gameManager->AddCoin(1);
  };

  auto sprite = coin->AddComponent<SpriteRenderer>();
  sprite->AddAnimation("Idle", "../assets/source.png", 288, 272, 8, 8, 4, 0.1f,
                       true);

  return coin;
}
} // namespace Prefabs
