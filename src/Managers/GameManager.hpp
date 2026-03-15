#pragma once

#include "../Core/World.hpp"
#include "raylib.h"

class GameManager {
private:
  GameManager();
  ~GameManager();

public:
  static GameManager &Get();

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

  void Init();
  void DevGen();
  void GenEnemy(int count);
  void HandleGlobalEvents();
  void AddShock(Vector2 absolutePos);
  void Update();
  void DrawUI();
  void Draw();
  void Clear();
  void AddCoin(int amount);
};
