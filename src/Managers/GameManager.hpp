#pragma once

#include "../Core/Components/DamageText.hpp"
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

  //--- shader related variables ---
  Shader shader = {0};
  int timeLoc;
  int centresLoc;
  int timesLoc;
  int aspectRatioLoc;

  static const int NUM_SHOCKWAVES = 20;
  Vector2 shockCentres[NUM_SHOCKWAVES];
  float shockTimes[NUM_SHOCKWAVES];
  RenderTexture2D renderTexture;
  // ---- shader end ----

  // Game State
  int coint = 0;

  void Init() {

    ResourceManager::GetInstance().GetTexture("../assets/source.png");

    shader = LoadShader(nullptr, "../assets/shaders/world.frag");

    // Get shader uniform locations
    timeLoc = GetShaderLocation(shader, "time");
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

    world.cm = &cm;

    player = Prefabs::CreatePlayer(world, {0, 0});

    cm.SetTarget(&player->position);

    GenEnemy(10);
    DevGen();

    world.gameManager = this;
  }

  void DevGen() {
    // auto obj = world.CreateObject("text");
    // obj->AddComponent<DamageText>("Hello World!", RED, 30);
  }

  void GenEnemy(int count) {
    for (int i = 0; i < count; i++) {
      Vector2 pos =
          MathUtils::GetRandomArroundPosition(player->position, 300.0f, 700.0f);
      GameObject *enemy1 = Prefabs::CreateKnight(world, pos, player);
    }
  }

  void AddShock(Vector2 absolutePos) {
    Vector2 screenPosition = GetWorldToScreen2D(absolutePos, cm.GetCamera());
    int selectedIndex = -1;
    float oldestTime = -1.0f;

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

  void Update(float dt) {
    world.Update(dt);
    world.ResolveCollisions();
    cm.Update(dt);

    float time = GetTime();

    // if (IsKeyPressed(KEY_SPACE)) {
    //   Vector2 randomScreenPos = {
    //       (float)GetRandomValue(0, GetScreenWidth() - 1),
    //       (float)GetRandomValue(0, GetScreenHeight() - 1)};
    //   AddShock(randomScreenPos);
    // }

    for (int i = 0; i < NUM_SHOCKWAVES; i++) {
      if (shockTimes[i] < 1.0f) {
        shockTimes[i] += dt;
      }
    }

    SetShaderValueV(shader, centresLoc, shockCentres, SHADER_UNIFORM_VEC2,
                    NUM_SHOCKWAVES);
    SetShaderValueV(shader, timesLoc, shockTimes, SHADER_UNIFORM_FLOAT,
                    NUM_SHOCKWAVES);

    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    if (IsKeyPressed(KEY_TAB)) {
      int randomIdx = GetRandomValue(0, world.objects.size() - 1);
      auto &randomTarget = world.objects[randomIdx];
      cm.SetTarget(&randomTarget->position);
    }
    if (IsKeyPressed(KEY_R)) {
      GenEnemy(20);
    }
  }

  void DrawUI() {
    DrawText("Use WASD to move", 10, 30, 20, DARKGRAY);

    for (auto &obj : world.objects) {
      obj->DrawUI();
    }

    DrawText(TextFormat("Coins: %d", coint), 10, 50, 20, GOLD);
  }

  void Draw() {

    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);
    BeginMode2D(cm.GetCamera());
    DrawText("Map", 0, 0, 40, BLACK);
    world.Draw();
    cm.Draw();
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

  void AddCoin(int amount) { coint += amount; }
};
