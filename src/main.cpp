#include "Managers/GameManager.hpp"
#include "raylib.h"

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Timmy Survival");

  GameManager gm;
  gm.Init();

  while (!WindowShouldClose()) {
    gm.Update();
    gm.Draw();
  }
  gm.Clear();
  CloseWindow();
  return 0;
}
