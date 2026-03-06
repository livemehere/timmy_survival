#include "Prefabs.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/PlayerController.hpp"
#include "GameObject.hpp"

namespace Prefabs {

GameObject *CreatePlayer(World &world, Vector2 position) {
  GameObject *player = world.CreateObject("player1");
  player->position = position;
  player->AddComponent<BoxCollider>(50.0f, 50.0f, -25.0f, -25.0f);
  player->AddComponent<PlayerController>(150.0f);
  return player;
}
} // namespace Prefabs
