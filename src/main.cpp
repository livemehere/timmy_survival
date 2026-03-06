#include "Core/GameObject.hpp"
#include "Core/Prefabs.hpp"
#include "Core/World.hpp"
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

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    world.Update(dt);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawFPS(10, 10);
    DrawText("Hello World", 600, 400, 40, BLACK);
    world.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
