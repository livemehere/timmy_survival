#pragma once

#include "../Core/Timer.hpp"
#include "../Core/World.hpp"
#include "../Prefabs/Types/EffectDefinition.hpp"
#include "../Prefabs/Types/StageDefinition.hpp"
#include "../Prefabs/Types/WeaponDefinition.hpp"
#include "PlayerProgression.hpp"
#include "ShopTypes.hpp"
#include "raylib.h"
#include <vector>

class GameManager {
private:
  GameManager() = default;
  ~GameManager() = default;

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
  GameObject *player = nullptr;
  float gameSpeed = 0.85f;
  int coin = 0;
  int currentStageIndex = -1;
  int stageRepeatCount = 0; // Current base stage repeat count
  int currentBossCount = 0; // Track spawned bosses for limiting
  int stageTargetSpawnCount = 0;
  int stageSpawnedCount = 0;
  float nextStageSpawnIn = 0.0f;
  float stageBannerTimer = 0.0f;
  bool isCameraFollowingPlayer = true;
  bool stageSpawnComplete = false; // All monsters spawned flag
  bool gameOver = false;           // Game over flag
  bool gameCleared = false;
  bool shopOpen = false;
  int shopRerollCost = 4;
  Timer damageBurstCooldownTimer = Timer(0.0f, false);
  Timer damageZoneCooldownTimer = Timer(0.0f, false);
  Timer coinMagnetTimer = Timer(60.0f, false); // 1-minute (60-second) cooldown
  PlayerProgression progression;
  std::vector<ShopOffer> shopOffers;
  std::vector<GameObject *> energyBallWeapons;
  std::vector<GameObject *> homingMissileWeapons;
  GameObject *orbitWeaponObject = nullptr;

  // Infinite stage scaling
  float currentHealthScale = 1.0f;
  float currentSpawnScale = 1.0f;
  float currentSpeedScale = 1.0f;
  StageDefinition currentDynamicStage; // Dynamically generated stage

  void Init();
  void DevGen();
  void GenEnemy(int count);
  void ClearEnemies();
  void StartStage(int stageIndex);
  void UpdateStage(float dt);
  void SpawnStageEnemy();
  const StageDefinition *GetCurrentStageDefinition() const;
  void RefreshPlayerLoadout();
  void UpdatePlayerStats();
  void RestartGame(); // Restart game
  WeaponDefinition BuildEnergyBallDefinition(int index) const;
  WeaponDefinition BuildOrbitWeaponDefinition() const;
  WeaponDefinition BuildHomingMissileDefinition(int index) const;
  EffectDefinition BuildDamageBurstDefinition() const;
  EffectDefinition BuildDamageZoneDefinition() const;
  float GetDamageBurstCooldown() const;
  float GetDamageZoneCooldown() const;
  void ToggleShop();
  void RollShopOffers();
  bool CanOfferItem(ShopItemId itemId) const;
  int GetShopItemWeight(ShopItemId itemId) const;
  ShopOffer BuildShopOffer(ShopItemId itemId) const;
  bool PurchaseShopOffer(int offerIndex);
  bool RerollShopOffers();
  void DrawShopOverlay();
  void HandleGlobalEvents();
  void AddShock(Vector2 absolutePos);
  void Update();
  void DrawUI();
  void Draw();
  void Clear();
  void AddCoin(int amount);
};
