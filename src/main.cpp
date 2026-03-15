#include "Managers/GameManager.hpp"
#include "Managers/CameraManager.hpp"
#include "Managers/ResourceManager.hpp"
#include "raylib.h"

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Timmy Survival");

  GameManager &gm = GameManager::Get();
  CameraManager &cm = CameraManager::Get();
  ResourceManager &rm = ResourceManager::Get();

  cm.Init();
  gm.Init();
  rm.Init();

  while (!WindowShouldClose()) {
    gm.Update();
    gm.Draw();
  }

  gm.Clear();
  cm.Clear();
  rm.Clear();

  CloseWindow();
  return 0;
}
