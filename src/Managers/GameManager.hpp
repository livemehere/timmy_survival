#pragma once

#include "../Core/Prefabs.hpp"
#include "../Core/World.hpp"
#include "../Utils/MathUtils.hpp"
#include "CameraManager.hpp"
#include "ResourceManager.hpp"
#include "raylib.h"

class GameManager {
public:
  World world;
  GameObject *player;
  CameraManager cm;

  void Init() {

    ResourceManager::GetInstance().GetTexture("../assets/source.png");
    world.cm = &cm;

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

    if (IsKeyPressed(KEY_TAB)) {
      int randomIdx = GetRandomValue(0, world.objects.size() - 1);
      auto &randomTarget = world.objects[randomIdx];
      cm.SetTarget(&randomTarget->position);
    }
    if (IsKeyPressed(KEY_R)) {
      GenEnemy(20);
    }
  }

  void DrawUI() { DrawText("Use WASD to move", 10, 30, 20, DARKGRAY); }

  void Draw() {

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(cm.GetCamera());

    DrawText("Map", 0, 0, 40, BLACK);
    world.Draw();
    cm.Draw();

    EndMode2D();

    DrawUI();

    EndDrawing();
  }
};
