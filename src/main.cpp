#include "Managers/GameManager.hpp"
#include "raylib.h"

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;
constexpr int screenCenterWidth = screenWidth / 2;
constexpr int screenCenterHeight = screenHeight / 2;

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Timmy Survival");

  GameManager gm;
  gm.Init();

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    gm.Update(dt);
    gm.Draw();
  }

  CloseWindow();
  return 0;
}
