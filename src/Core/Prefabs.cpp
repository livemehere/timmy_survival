#include "Prefabs.hpp"
#include "../Managers/GameManager.hpp";
#include "Components/CircleCollider.hpp"
#include "Components/EnemyAI.hpp"
#include "Components/FireWeapon.hpp"
#include "Components/Health.hpp"
#include "Components/Knockback.hpp"
#include "Components/PlayerController.hpp"
#include "Components/SpriteRenderer.hpp"
#include "GameObject.hpp"
#include <string>

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position) {
  GameObject *player = world.CreateObject("player1");
  player->position = position;
  player->AddComponent<CircleCollider>(8.0f);
  player->AddComponent<PlayerController>(50.0f);

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
  // TODO: drop item with random chance on death
  health->onDeath = [knight, &world]() {
    knight->world->gameManager->AddShock(knight->position);
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
} // namespace Prefabs
