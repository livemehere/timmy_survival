#pragma once

#include "../Core/World.hpp"
#include "../Prefabs/Prefabs.hpp"
#include "../Utils/MathUtils.hpp"
#include "CameraManager.hpp"
#include "raylib.h"

class GameManager {
private:
  GameManager() {}
  ~GameManager() {}

public:
  static GameManager &Get() {
    static GameManager instance;
    return instance;
  }

  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;

  //--- shader related variables ---
  Shader shader = {0};
  int centresLoc;
  int timesLoc;
  int aspectRatioLoc;

  static const int NUM_SHOCKWAVES = 20;
  Vector2 shockCentres[NUM_SHOCKWAVES];
  float shockTimes[NUM_SHOCKWAVES];
  RenderTexture2D renderTexture;
  // ---- shader end ----

  // Game State
  World world;
  GameObject *player;
  float gameSpeed = 1.0f;
  int coin = 0;

  void Init() {

    shader = LoadShader(nullptr, "../assets/shaders/world.frag");

    // Get shader uniform locations
    centresLoc = GetShaderLocation(shader, "centres");
    timesLoc = GetShaderLocation(shader, "times");
    aspectRatioLoc = GetShaderLocation(shader, "aspectRatio");

    // initialize shockwave data
    for (int i = 0; i < NUM_SHOCKWAVES; i++) {
      shockTimes[i] = 999.0f;
      shockCentres[i] = {0.0f, 0.0f};
    }

    float aspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
    SetShaderValue(shader, aspectRatioLoc, &aspectRatio, SHADER_UNIFORM_FLOAT);

    renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    // player setup
    player = Prefabs::CreatePlayer(world, {0, 0});
    CameraManager::Get().SetTarget(&player->position);

    // for development testing
    DevGen();
  }

  void DevGen() { GenEnemy(10); }

  void GenEnemy(int count) {
    for (int i = 0; i < count; i++) {
      Vector2 pos =
          MathUtils::GetRandomArroundPosition(player->position, 300.0f, 700.0f);
      GameObject *enemy1 = Prefabs::CreateKnight(world, pos, player);
    }
  }

  void HandleGlobalEvents() {
    if (IsKeyPressed(KEY_TAB)) {
      int randomIdx = GetRandomValue(0, world.objects.size() - 1);
      auto &randomTarget = world.objects[randomIdx];
      CameraManager::Get().SetTarget(&randomTarget->position);
    }
    if (IsKeyPressed(KEY_R)) {
      GenEnemy(20);
    }
    if (IsKeyPressed(KEY_LEFT)) {
      gameSpeed = std::max(0.1f, gameSpeed - 0.1f);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
      gameSpeed = std::min(5.0f, gameSpeed + 0.1f);
    }
  }

  void AddShock(Vector2 absolutePos) {
    Vector2 screenPosition =
        GetWorldToScreen2D(absolutePos, CameraManager::Get().GetCamera());
    int selectedIndex = -1;
    float oldestTime = -1.0f;

    // find available shockwave slot or the oldest one
    for (int i = 0; i < NUM_SHOCKWAVES; i++) {
      if (shockTimes[i] >= 1.0f) {
        selectedIndex = i;
        break;
      }

      if (shockTimes[i] > oldestTime) {
        oldestTime = shockTimes[i];
        selectedIndex = i;
      }
    }

    if (selectedIndex < 0) {
      return;
    }

    shockTimes[selectedIndex] = 0.0f;
    shockCentres[selectedIndex].x = screenPosition.x / (float)GetScreenWidth();
    shockCentres[selectedIndex].y =
        1.0f - (screenPosition.y / (float)GetScreenHeight());
  }

  void Update() {
    float dt = GetFrameTime();
    float scaledDt = dt * gameSpeed;

    // world update
    world.Update(scaledDt);
    world.ResolveCollisions();

    // camera update
    CameraManager::Get().Update(dt);

    // shockwave update
    for (int i = 0; i < NUM_SHOCKWAVES; i++) {
      if (shockTimes[i] < 1.0f) {
        shockTimes[i] += dt;
      }
    }
    SetShaderValueV(shader, centresLoc, shockCentres, SHADER_UNIFORM_VEC2,
                    NUM_SHOCKWAVES);
    SetShaderValueV(shader, timesLoc, shockTimes, SHADER_UNIFORM_FLOAT,
                    NUM_SHOCKWAVES);

    HandleGlobalEvents();
  }

  void DrawUI() {
    DrawText("Use WASD to move", 10, 30, 20, DARKGRAY);

    for (auto &obj : world.objects) {
      obj->DrawUI();
    }

    DrawText(TextFormat("Coins: %d", coin), 10, 50, 20, GOLD);
  }

  void Draw() {

    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);
    BeginMode2D(CameraManager::Get().GetCamera());
    DrawText("Map", 0, 0, 40, BLACK);
    world.Draw();
    CameraManager::Get().Draw();
    EndMode2D();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    BeginShaderMode(shader);
    Rectangle sourceRec = {0, 0,
                           static_cast<float>(renderTexture.texture.width),
                           static_cast<float>(-renderTexture.texture.height)};
    DrawTextureRec(renderTexture.texture, sourceRec, {0, 0}, WHITE);
    EndShaderMode();

    // ---- out of shader effects ----
    DrawUI();
    DrawFPS(10, 10);
    // -------------------------------

    EndDrawing();
  }

  void Clear() {
    if (shader.id != 0) {
      UnloadShader(shader);
      shader.id = 0;
    }
  }

  void AddCoin(int amount) { coin += amount; }
};
