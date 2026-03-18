#include "Managers/GameManager.hpp"
#include "Managers/CameraManager.hpp"
#include "Managers/ResourceManager.hpp"
#include "raylib.h"

#if defined(__EMSCRIPTEN__) && __has_include(<emscripten/emscripten.h>)
#include <emscripten/emscripten.h>
#endif

constexpr int screenWidth = 1600;
constexpr int screenHeight = 900;

namespace {
GameManager *gGameManager = nullptr;
CameraManager *gCameraManager = nullptr;
ResourceManager *gResourceManager = nullptr;

void RunFrame() {
  gGameManager->Update();
  gGameManager->Draw();
}

void Shutdown() {
  gGameManager->Clear();
  gCameraManager->Clear();
  gResourceManager->Clear();
  CloseWindow();
}
} // namespace

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Timmy Survival");

  GameManager &gm = GameManager::Get();
  CameraManager &cm = CameraManager::Get();
  ResourceManager &rm = ResourceManager::Get();

  gGameManager = &gm;
  gCameraManager = &cm;
  gResourceManager = &rm;

  cm.Init();
  gm.Init();
  rm.Init();

#if defined(__EMSCRIPTEN__) && __has_include(<emscripten/emscripten.h>)
  emscripten_set_main_loop(RunFrame, 0, 1);
  return 0;
#else
  while (!WindowShouldClose()) {
    RunFrame();
  }

  Shutdown();
  return 0;
#endif
}
