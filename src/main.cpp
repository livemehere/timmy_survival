#include "Core/Components/BoxCollider.hpp"
#include "Core/Components/PlayerController.hpp"
#include "Core/World.hpp"
#include "raylib.h"

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;
constexpr int screenCenter = screenWidth / 2;
constexpr int screenMiddle = screenHeight / 2;

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Timmy Survival");

  World world;

  GameObject *player = world.CreateObject("Player");
  player->AddComponent<BoxCollider>(50.0f, 50.0f);
  player->AddComponent<PlayerController>(180.0f);
  player->position = {screenCenter, screenMiddle};

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
