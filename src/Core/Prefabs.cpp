#include "Prefabs.hpp"
#include "Components/CircleCollider.hpp"
#include "Components/EnemyAI.hpp"
#include "Components/PlayerController.hpp"
#include "Components/SpriteRenderer.hpp"
#include "GameObject.hpp"

namespace Prefabs {
GameObject *CreatePlayer(World &world, Vector2 position) {
  GameObject *player = world.CreateObject("player1");
  player->position = position;
  player->AddComponent<CircleCollider>(8.0f);
  player->AddComponent<PlayerController>(50.0f);

  auto sprite = player->AddComponent<SpriteRenderer>();
  sprite->AddAnimation("Idle", "assets/source.png", 128, 32, 16, 32, 2, 0.5f,
                       true);
  sprite->anchorRatio = {0.5f, 0.75f};
  //  sprite->Play("Idle");

  return player;
}

GameObject *CreateKnight(World &world, Vector2 position, GameObject *target) {
  GameObject *knight = world.CreateObject("knight");
  knight->position = position;

  knight->AddComponent<CircleCollider>(8.0f);
  knight->AddComponent<EnemyAI>(target, 35.0f);

  return knight;
}
} // namespace Prefabs
