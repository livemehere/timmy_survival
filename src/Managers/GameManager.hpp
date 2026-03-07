#pragma once

#include "../Core/Prefabs.hpp"
#include "../Core/World.hpp"
#include "../Utils/MathUtils.hpp"
#include "CameraManager.hpp"
#include "raylib.h"

class GameManager {
public:
  World world;
  GameObject *player;
  CameraManager cm;

  void Init() {
    player = Prefabs::CreatePlayer(world, {0, 0});

    cm.SetTarget(&player->position);
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
    cm.Update(dt);
  }

  void DrawUI() { DrawFPS(10, 10); }

  void Draw() {

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(cm.GetCamera());
    DrawText("Map", 0, 0, 40, BLACK);
    world.Draw();
    EndMode2D();

    DrawUI();

    EndDrawing();
  }
};
