#pragma once

#include "../Core/Prefabs.hpp"
#include "../Core/World.hpp"
#include "../Utils/MathUtils.hpp"

class GameManager {
public:
  World world;
  GameObject *player;

  void Init() {
    player = Prefabs::CreatePlayer(world, {0, 0});

    GenEnemy(10);
  }

  void GenEnemy(int count) {
    for (int i = 0; i < count; i++) {
      Vector2 pos =
          MathUtils::GetRandomArroundPosition(player->position, 300.0f, 700.0f);
      GameObject *enemy1 = Prefabs::CreateKnight(world, pos, player);
    }
  }

  void Update(float dt) {
    world.Update(dt);
    world.ResolveCollisions();
  }

  void Draw() {

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawFPS(10, 10);
    DrawText("Map", 600, 400, 40, BLACK);
    world.Draw();

    EndDrawing();
  }
};
