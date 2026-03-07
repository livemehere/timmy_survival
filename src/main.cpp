#include "Core/GameObject.hpp"
#include "Core/Prefabs.hpp"
#include "Core/World.hpp"
#include "Utils/MathUtils.hpp"
#include "raylib.h"

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;
constexpr int screenCenterWidth = screenWidth / 2;
constexpr int screenCenterHeight = screenHeight / 2;

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Timmy Survival");

  World world;

  GameObject *player =
      Prefabs::CreatePlayer(world, {screenCenterWidth, screenCenterHeight});

  for (int i = 0; i < 10; i++) {
    Vector2 pos =
        MathUtils::GetRandomArroundPosition(player->position, 300.0f, 700.0f);
    GameObject *enemy1 = Prefabs::CreateKnight(world, pos, player);
  }

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    world.Update(dt);
    world.ResolveCollisions();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawFPS(10, 10);
    DrawText("Map", 600, 400, 40, BLACK);
    world.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
