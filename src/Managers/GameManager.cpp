#include "GameManager.hpp"
#include "../Components/Colliders/CircleCollider.hpp"
#include "../Components/Health.hpp"
#include "../Components/Movement/Magnet.hpp"
#include "../Components/PlayerController.hpp"
#include "../Components/Weapons/FireWeapon.hpp"
#include "../Components/Weapons/OrbitWeapon.hpp"
#include "../Prefabs/Definitions/Effects.hpp"
#include "../Prefabs/Definitions/Enemies.hpp"
#include "../Prefabs/Definitions/Stages.hpp"
#include "../Prefabs/Definitions/Weapons.hpp"
#include "../Prefabs/Prefabs.hpp"
#include "../Utils/MathUtils.hpp"
#include "CameraManager.hpp"
#include "ResourceManager.hpp"
#include "ShopItemData.hpp"
#include "ShopTypes.hpp"
#include "raylib.h"
#include "raymath.h"

#include <algorithm>
#include <array>

namespace {
constexpr float kStageBannerDuration = 2.2f;
constexpr float kShopPanelWidth = 980.0f;
constexpr float kShopPanelHeight = 520.0f;

const std::array<ShopItemId, 29> kAllShopItems = {
    ShopItemId::ADD_ENERGY_BALL,
    ShopItemId::ENERGY_BALL_ATTACK_SPEED,
    ShopItemId::ENERGY_BALL_DAMAGE_UP,
    ShopItemId::ENERGY_BALL_RANGE_UP,
    ShopItemId::ENERGY_BALL_PIERCE_UP,
    ShopItemId::ENERGY_BALL_KNOCKBACK_UP,
    ShopItemId::UNLOCK_ORBIT_WEAPON,
    ShopItemId::ADD_ORBIT_BLADE,
    ShopItemId::ORBIT_SPEED_UP,
    ShopItemId::ORBIT_DAMAGE_UP,
    ShopItemId::UNLOCK_HOMING_MISSILE,
    ShopItemId::ADD_HOMING_MISSILE,
    ShopItemId::HOMING_MISSILE_DAMAGE_UP,
    ShopItemId::HOMING_MISSILE_SPEED_UP,
    ShopItemId::UNLOCK_DAMAGE_BURST,
    ShopItemId::DAMAGE_BURST_DAMAGE_UP,
    ShopItemId::DAMAGE_BURST_DELAY_DOWN,
    ShopItemId::DAMAGE_BURST_SIZE_UP,
    ShopItemId::DAMAGE_BURST_KEEPING_COUNT_UP,
    ShopItemId::UNLOCK_DAMAGE_ZONE,
    ShopItemId::DAMAGE_ZONE_DAMAGE_UP,
    ShopItemId::DAMAGE_ZONE_DURATION_UP,
    ShopItemId::DAMAGE_ZONE_TICKRATE_UP,
    ShopItemId::DAMAGE_ZONE_SIZE_UP,
    ShopItemId::DAMAGE_ZONE_KEEPING_COUNT_UP,
    ShopItemId::MOVE_SPEED_UP,
    ShopItemId::PICKUP_RADIUS_UP,
    ShopItemId::HEALTH_RECOVERY_SMALL,
    ShopItemId::HEALTH_RECOVERY_LARGE,
};

const char *GetCategoryLabel(ShopCategory category) {
  switch (category) {
  case ShopCategory::WEAPON:
    return "Weapon";
  case ShopCategory::STAT:
    return "Stat";
  case ShopCategory::SKILL:
    return "Skill";
  }

  return "Unknown";
}

std::vector<std::string> WrapTextLines(const std::string &text, int fontSize,
                                       float maxWidth) {
  std::vector<std::string> lines;
  std::string currentLine;
  std::string currentWord;

  auto flushWord = [&]() {
    if (currentWord.empty()) {
      return;
    }

    std::string candidate =
        currentLine.empty() ? currentWord : currentLine + " " + currentWord;
    if (!currentLine.empty() &&
        MeasureText(candidate.c_str(), fontSize) > maxWidth) {
      lines.push_back(currentLine);
      currentLine = currentWord;
    } else {
      currentLine = candidate;
    }

    currentWord.clear();
  };

  for (char ch : text) {
    if (ch == ' ') {
      flushWord();
      continue;
    }

    if (ch == '\n') {
      flushWord();
      if (!currentLine.empty()) {
        lines.push_back(currentLine);
        currentLine.clear();
      }
      continue;
    }

    currentWord += ch;
  }

  flushWord();
  if (!currentLine.empty()) {
    lines.push_back(currentLine);
  }

  if (lines.empty()) {
    lines.push_back("");
  }

  return lines;
}

float GetSpawnInterval(const StageDefinition &stage, int targetSpawnCount,
                       int spawnedCount, float elapsed) {
  int remainingSpawns = std::max(1, targetSpawnCount - spawnedCount);
  float remainingTime = std::max(0.25f, stage.duration - elapsed);
  float baseInterval = remainingTime / remainingSpawns;
  float jitter = stage.spawnIntervalJitter;
  return std::max(
      0.12f, baseInterval * MathUtils::GetRandom(1.0f - jitter, 1.0f + jitter));
}
} // namespace

GameManager &GameManager::Get() {
  static GameManager instance;
  return instance;
}

WeaponDefinition GameManager::BuildEnergyBallDefinition(int index) const {
  auto definition = WeaponDefinitions::ENERGY_BALL;
  definition.name = TextFormat("weapon_energyBall_%d", index);
  definition.followOffset = {index == 0 ? -10.0f : 10.0f, -10.0f};
  definition.cooldown *=
      std::max(0.42f, 1.0f - 0.08f * progression.energyBallAttackSpeedLevel);
  definition.damage += progression.energyBallDamageLevel * 0.45f;
  definition.range *= (1.0f + 0.12f * progression.energyBallRangeLevel);
  definition.projectileDefinition.pierce +=
      progression.energyBallProjectilePierceLevel;
  definition.projectileDefinition.knockbackForce *=
      (1.0f + 0.15f * progression.energyBallProjectileKnockbackLevel);
  return definition;
}

WeaponDefinition GameManager::BuildOrbitWeaponDefinition() const {
  auto definition = WeaponDefinitions::ORBIT_BLADE;
  definition.count = progression.orbitCount;
  definition.damage += progression.orbitDamageLevel * 0.35f;
  definition.orbitSpeedDegrees *= (1.0f + 0.08f * progression.orbitSpeedLevel);
  return definition;
}

WeaponDefinition GameManager::BuildHomingMissileDefinition(int index) const {
  auto definition = WeaponDefinitions::HOMING_MISSILE;
  definition.name = TextFormat("weapon_homingMissile_%d", index);
  definition.followOffset = {10.0f + index * 15.0f, -10.0f};
  definition.cooldown *=
      std::max(0.55f, 1.0f - 0.06f * progression.homingMissileSpeedLevel);
  definition.damage += progression.homingMissileDamageLevel * 0.3f;
  definition.projectileDefinition.speed *=
      (1.0f + 0.08f * progression.homingMissileSpeedLevel);
  definition.projectileDefinition.homingTurnRate *=
      (1.0f + 0.08f * progression.homingMissileSpeedLevel);
  definition.range *= (1.0f + 0.08f * progression.homingMissileSpeedLevel);
  return definition;
}

EffectDefinition GameManager::BuildDamageBurstDefinition() const {
  auto definition = EffectDefinitions::DAMAGE_BURST;
  definition.damage *= (1.0f + progression.damageBurstDamageLevel * 0.25f);
  definition.hitDelay = std::max(
      0.05f, definition.hitDelay - progression.damageBurstDelayLevel * 0.02f);
  definition.width *= (1.0f + progression.damageBurstSizeLevel * 0.12f);
  definition.height *= (1.0f + progression.damageBurstSizeLevel * 0.12f);
  return definition;
}

EffectDefinition GameManager::BuildDamageZoneDefinition() const {
  auto definition = EffectDefinitions::DAMAGE_ZONE;
  definition.damage *= (1.0f + progression.damageZoneDamageLevel * 0.2f);
  definition.lifetime *= (1.0f + progression.damageZoneDurationLevel * 0.15f);
  definition.tickInterval = std::max(
      0.12f, definition.tickInterval - progression.damageZoneTickLevel * 0.03f);
  definition.width *= (1.0f + progression.damageZoneSizeLevel * 0.12f);
  definition.height *= (1.0f + progression.damageZoneSizeLevel * 0.12f);
  return definition;
}

float GameManager::GetDamageBurstCooldown() const {
  return std::max(0.4f, 3.2f - progression.damageBurstDelayLevel * 0.18f);
}

float GameManager::GetDamageZoneCooldown() const {
  return std::max(0.6f, 5.0f - progression.damageZoneTickLevel * 0.12f);
}

void GameManager::UpdatePlayerStats() {
  if (!player) {
    return;
  }

  auto controller = player->GetComponent<PlayerController>();
  if (controller) {
    controller->speed = 140.0f * (1.0f + 0.07f * progression.moveSpeedLevel);
  }

  for (auto &component : player->components) {
    auto *collider = dynamic_cast<CircleCollider *>(component.get());
    if (collider && collider->label.has_value() &&
        collider->label.value() == ColliderLabel::PLAYER_PICKUP) {
      collider->radius = 50.0f * (1.0f + 0.15f * progression.pickupRadiusLevel);
    }
  }
}

void GameManager::RefreshPlayerLoadout() {
  if (!player) {
    return;
  }

  UpdatePlayerStats();

  energyBallWeapons.erase(std::remove_if(energyBallWeapons.begin(),
                                         energyBallWeapons.end(),
                                         [](GameObject *weapon) {
                                           return !weapon || !weapon->isAlive;
                                         }),
                          energyBallWeapons.end());

  while (static_cast<int>(energyBallWeapons.size()) >
         progression.energyBallCount) {
    energyBallWeapons.back()->Destroy();
    energyBallWeapons.pop_back();
  }

  while (static_cast<int>(energyBallWeapons.size()) <
         progression.energyBallCount) {
    int index = static_cast<int>(energyBallWeapons.size());
    energyBallWeapons.push_back(
        Prefabs::CreateWeapon(world, player, BuildEnergyBallDefinition(index)));
  }

  for (size_t i = 0; i < energyBallWeapons.size(); i++) {
    auto *weaponObject = energyBallWeapons[i];
    if (!weaponObject || !weaponObject->isAlive) {
      continue;
    }

    auto *fireWeapon = weaponObject->GetComponent<FireWeapon>();
    if (fireWeapon) {
      auto definition = BuildEnergyBallDefinition(static_cast<int>(i));
      fireWeapon->damage = definition.damage;
      fireWeapon->range = definition.range;
      fireWeapon->timer.Reset(definition.cooldown);
      fireWeapon->projectileDefinition = definition.projectileDefinition;
    }
  }

  if (!progression.orbitUnlocked) {
    if (orbitWeaponObject && orbitWeaponObject->isAlive) {
      orbitWeaponObject->Destroy();
    }
    orbitWeaponObject = nullptr;
    return;
  }

  if (!orbitWeaponObject || !orbitWeaponObject->isAlive) {
    orbitWeaponObject =
        Prefabs::CreateWeapon(world, player, BuildOrbitWeaponDefinition());
  }

  auto *orbitWeapon = orbitWeaponObject->GetComponent<OrbitWeapon>();
  if (orbitWeapon) {
    auto definition = BuildOrbitWeaponDefinition();
    orbitWeapon->count = definition.count;
    orbitWeapon->damage = definition.damage;
    orbitWeapon->orbitSpeedDegrees = definition.orbitSpeedDegrees;
  }

  // Manage homing missiles
  if (!progression.homingMissileUnlocked) {
    for (auto *weapon : homingMissileWeapons) {
      if (weapon && weapon->isAlive) {
        weapon->Destroy();
      }
    }
    homingMissileWeapons.clear();
    return;
  }

  // Remove destroyed weapons
  homingMissileWeapons.erase(
      std::remove_if(
          homingMissileWeapons.begin(), homingMissileWeapons.end(),
          [](GameObject *weapon) { return !weapon || !weapon->isAlive; }),
      homingMissileWeapons.end());

  // Remove extras
  while (static_cast<int>(homingMissileWeapons.size()) >
         progression.homingMissileCount) {
    homingMissileWeapons.back()->Destroy();
    homingMissileWeapons.pop_back();
  }

  // Add missing weapons
  while (static_cast<int>(homingMissileWeapons.size()) <
         progression.homingMissileCount) {
    int index = static_cast<int>(homingMissileWeapons.size());
    homingMissileWeapons.push_back(Prefabs::CreateWeapon(
        world, player, BuildHomingMissileDefinition(index)));
  }

  // Update weapon stats
  for (size_t i = 0; i < homingMissileWeapons.size(); i++) {
    auto *weaponObject = homingMissileWeapons[i];
    if (!weaponObject || !weaponObject->isAlive) {
      continue;
    }

    auto *fireWeapon = weaponObject->GetComponent<FireWeapon>();
    if (fireWeapon) {
      auto definition = BuildHomingMissileDefinition(static_cast<int>(i));
      fireWeapon->damage = definition.damage;
      fireWeapon->range = definition.range;
      fireWeapon->timer.Reset(definition.cooldown);
      fireWeapon->projectileDefinition = definition.projectileDefinition;
    }
  }
}

bool GameManager::CanOfferItem(ShopItemId itemId) const {
  auto it = kShopItemData.find(itemId);
  if (it == kShopItemData.end()) {
    return false;
  }

  const ShopItemConfig &config = it->second;
  return config.canOfferCondition(progression);
}

int GameManager::GetShopItemWeight(ShopItemId itemId) const {
  auto it = kShopItemData.find(itemId);
  if (it == kShopItemData.end()) {
    return 10; // Default value
  }

  return it->second.weight;
}

ShopOffer GameManager::BuildShopOffer(ShopItemId itemId) const {
  auto it = kShopItemData.find(itemId);
  if (it == kShopItemData.end()) {
    return {itemId, ShopCategory::STAT, "Unknown", "", 999};
  }

  const ShopItemConfig &config = it->second;

  ShopOffer offer;
  offer.id = config.id;
  offer.category = config.category;
  offer.title = config.title;
  offer.description = config.description;
  offer.cost = config.cost;
  offer.isWrappedCached = false; // Start in uncached state

  return offer;
}

void GameManager::RollShopOffers() {
  shopOffers.clear();
  std::vector<ShopItemId> candidates;
  for (ShopItemId itemId : kAllShopItems) {
    if (CanOfferItem(itemId)) {
      candidates.push_back(itemId);
    }
  }

  while (!candidates.empty() && shopOffers.size() < 5) {
    int totalWeight = 0;
    for (ShopItemId itemId : candidates) {
      totalWeight += GetShopItemWeight(itemId);
    }

    int roll = MathUtils::GetRandom(1, std::max(1, totalWeight));
    int accum = 0;
    size_t selectedIndex = 0;
    for (size_t i = 0; i < candidates.size(); i++) {
      accum += GetShopItemWeight(candidates[i]);
      if (roll <= accum) {
        selectedIndex = i;
        break;
      }
    }

    shopOffers.push_back(BuildShopOffer(candidates[selectedIndex]));
    candidates.erase(candidates.begin() + static_cast<long>(selectedIndex));
  }
}

bool GameManager::PurchaseShopOffer(int offerIndex) {
  if (offerIndex < 0 || offerIndex >= static_cast<int>(shopOffers.size())) {
    return false;
  }

  const ShopOffer &offer = shopOffers[offerIndex];
  if (coin < offer.cost) {
    return false;
  }

  coin -= offer.cost;
  switch (offer.id) {
  case ShopItemId::ADD_ENERGY_BALL:
    progression.energyBallCount = std::min(2, progression.energyBallCount + 1);
    break;
  case ShopItemId::ENERGY_BALL_ATTACK_SPEED:
    progression.energyBallAttackSpeedLevel++;
    break;
  case ShopItemId::ENERGY_BALL_DAMAGE_UP:
    progression.energyBallDamageLevel++;
    break;
  case ShopItemId::ENERGY_BALL_RANGE_UP:
    progression.energyBallRangeLevel++;
    break;
  case ShopItemId::ENERGY_BALL_PIERCE_UP:
    progression.energyBallProjectilePierceLevel++;
    break;
  case ShopItemId::ENERGY_BALL_KNOCKBACK_UP:
    progression.energyBallProjectileKnockbackLevel++;
    break;
  case ShopItemId::UNLOCK_ORBIT_WEAPON:
    progression.orbitUnlocked = true;
    progression.orbitCount = std::max(1, progression.orbitCount);
    break;
  case ShopItemId::ADD_ORBIT_BLADE:
    progression.orbitCount = std::min(4, progression.orbitCount + 1);
    break;
  case ShopItemId::ORBIT_SPEED_UP:
    progression.orbitSpeedLevel++;
    break;
  case ShopItemId::ORBIT_DAMAGE_UP:
    progression.orbitDamageLevel++;
    break;
  case ShopItemId::UNLOCK_HOMING_MISSILE:
    progression.homingMissileUnlocked = true;
    progression.homingMissileCount =
        std::max(1, progression.homingMissileCount);
    break;
  case ShopItemId::ADD_HOMING_MISSILE:
    progression.homingMissileCount =
        std::min(2, progression.homingMissileCount + 1);
    break;
  case ShopItemId::HOMING_MISSILE_DAMAGE_UP:
    progression.homingMissileDamageLevel++;
    break;
  case ShopItemId::HOMING_MISSILE_SPEED_UP:
    progression.homingMissileSpeedLevel++;
    break;
  case ShopItemId::UNLOCK_DAMAGE_BURST:
    progression.damageBurstUnlocked = true;
    break;
  case ShopItemId::DAMAGE_BURST_DAMAGE_UP:
    progression.damageBurstDamageLevel++;
    break;
  case ShopItemId::DAMAGE_BURST_DELAY_DOWN:
    progression.damageBurstDelayLevel++;
    break;
  case ShopItemId::DAMAGE_BURST_SIZE_UP:
    progression.damageBurstSizeLevel++;
    break;
  case ShopItemId::DAMAGE_BURST_KEEPING_COUNT_UP:
    progression.damageBurstKeepingCount++;
    break;
  case ShopItemId::DAMAGE_ZONE_KEEPING_COUNT_UP:
    progression.damageZoneKeepingCount++;
    break;
  case ShopItemId::UNLOCK_DAMAGE_ZONE:
    progression.damageZoneUnlocked = true;
    break;
  case ShopItemId::DAMAGE_ZONE_DAMAGE_UP:
    progression.damageZoneDamageLevel++;
    break;
  case ShopItemId::DAMAGE_ZONE_DURATION_UP:
    progression.damageZoneDurationLevel++;
    break;
  case ShopItemId::DAMAGE_ZONE_TICKRATE_UP:
    progression.damageZoneTickLevel++;
    break;
  case ShopItemId::DAMAGE_ZONE_SIZE_UP:
    progression.damageZoneSizeLevel++;
    break;
  case ShopItemId::MOVE_SPEED_UP:
    progression.moveSpeedLevel++;
    break;
  case ShopItemId::PICKUP_RADIUS_UP:
    progression.pickupRadiusLevel++;
    break;
  case ShopItemId::HEALTH_RECOVERY_SMALL:
    if (player) {
      auto playerHealth = player->GetComponent<Health>();
      if (playerHealth) {
        playerHealth->hp =
            std::min(playerHealth->maxHp, playerHealth->hp + 25.0f);
      }
    }
    break;
  case ShopItemId::HEALTH_RECOVERY_LARGE:
    if (player) {
      auto playerHealth = player->GetComponent<Health>();
      if (playerHealth) {
        playerHealth->hp =
            std::min(playerHealth->maxHp, playerHealth->hp + 50.0f);
      }
    }
    break;
  }

  RefreshPlayerLoadout();
  RollShopOffers();
  return true;
}

bool GameManager::RerollShopOffers() {
  if (coin < shopRerollCost) {
    return false;
  }

  coin -= shopRerollCost;
  shopRerollCost += 2;
  RollShopOffers();
  return true;
}

void GameManager::ToggleShop() {
  shopOpen = !shopOpen;
  if (shopOpen) {
    shopRerollCost = 4;
    RollShopOffers();
  }
}

void GameManager::DrawShopOverlay() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));

  Rectangle panel = {(GetScreenWidth() - kShopPanelWidth) * 0.5f,
                     (GetScreenHeight() - kShopPanelHeight) * 0.5f,
                     kShopPanelWidth, kShopPanelHeight};
  DrawRectangleRounded(panel, 0.04f, 8, Fade(RAYWHITE, 0.98f));
  DrawRectangleRoundedLinesEx(panel, 0.04f, 8, 2.0f, Fade(BLACK, 0.2f));

  DrawText("SHOP", panel.x + 28, panel.y + 24, 36, BLACK);
  DrawText("Press Q to close", panel.x + 28, panel.y + 62, 18, DARKGRAY);
  DrawText(TextFormat("Coins: %d", coin), panel.x + 760, panel.y + 28, 26,
           GOLD);
  DrawText(TextFormat("Reroll [E]: %d", shopRerollCost), panel.x + 700,
           panel.y + 62, 20, MAROON);

  float cardWidth = 176.0f;
  float cardHeight = 320.0f;
  float cardGap = 12.0f;
  float startX = panel.x + 28;
  float startY = panel.y + 110;

  for (size_t i = 0; i < shopOffers.size(); i++) {
    ShopOffer &offer = shopOffers[i]; // Remove const: needed for caching
    Rectangle card = {startX + (cardWidth + cardGap) * i, startY, cardWidth,
                      cardHeight};
    Color cardColor = Fade(LIGHTGRAY, 0.75f);
    if (coin >= offer.cost) {
      cardColor = Fade(SKYBLUE, 0.2f);
    }

    DrawRectangleRounded(card, 0.06f, 6, cardColor);
    DrawRectangleRoundedLinesEx(card, 0.06f, 6, 2.0f, Fade(BLACK, 0.15f));

    DrawText(TextFormat("%d", static_cast<int>(i + 1)), card.x + 12,
             card.y + 10, 28, DARKGRAY);
    DrawText(GetCategoryLabel(offer.category), card.x + 12, card.y + 44, 18,
             DARKBLUE);
    DrawText(offer.title.c_str(), card.x + 12, card.y + 78, 22, BLACK);

    // Use cached text wrapping (rendering optimization)
    if (!offer.isWrappedCached) {
      offer.wrappedDescription =
          WrapTextLines(offer.description, 18, card.width - 24.0f);
      offer.isWrappedCached = true;
    }

    for (size_t lineIndex = 0; lineIndex < offer.wrappedDescription.size();
         lineIndex++) {
      DrawText(offer.wrappedDescription[lineIndex].c_str(), card.x + 12,
               card.y + 122 + static_cast<float>(lineIndex) * 22.0f, 18,
               DARKGRAY);
    }

    DrawText(TextFormat("Cost %d", offer.cost), card.x + 12, card.y + 280, 22,
             coin >= offer.cost ? GOLD : GRAY);
  }
}

void GameManager::Init() {

  const std::string shaderPath =
      ResourceManager::ResolvePath("assets/shaders/world.frag");
  shader = LoadShader(nullptr, shaderPath.c_str());

  centresLoc = GetShaderLocation(shader, "centres");
  timesLoc = GetShaderLocation(shader, "times");
  aspectRatioLoc = GetShaderLocation(shader, "aspectRatio");

  for (int i = 0; i < NUM_SHOCKWAVES; i++) {
    shockTimes[i] = 999.0f;
    shockCentres[i] = {0.0f, 0.0f};
  }

  float aspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
  SetShaderValue(shader, aspectRatioLoc, &aspectRatio, SHADER_UNIFORM_FLOAT);

  renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  // -- player --
  player = Prefabs::CreatePlayer(world, {0, 0});
  RefreshPlayerLoadout();
  gameSpeed = 1.0f;
  damageBurstCooldownTimer.Stop();
  damageZoneCooldownTimer.Stop();
  coinMagnetTimer.Reset(60.0f); // Start 1-minute coin magnet cooldown
  // -- end --

  CameraManager::Get().SetTarget(&player->position);
  isCameraFollowingPlayer = true;

  StartStage(0);
}

void GameManager::RestartGame() {
  // Reset all state
  currentStageIndex = -1;
  stageRepeatCount = 0;
  currentBossCount = 0;
  stageTargetSpawnCount = 0;
  stageSpawnedCount = 0;
  stageSpawnComplete = false;
  gameOver = false;
  gameCleared = false;
  shopOpen = false;
  gameSpeed = 1.0f;
  stageBannerTimer = kStageBannerDuration;
  nextStageSpawnIn = 0.1f;

  // Reset progression
  progression = PlayerProgression();

  // Reset weapon lists
  energyBallWeapons.clear();
  homingMissileWeapons.clear();
  if (orbitWeaponObject && orbitWeaponObject->isAlive) {
    orbitWeaponObject->Destroy();
  }
  orbitWeaponObject = nullptr;

  // Reset coins
  coin = 0;

  // Reset cooldowns
  damageBurstCooldownTimer.Stop();
  damageZoneCooldownTimer.Stop();
  coinMagnetTimer.Reset(60.0f);

  // Reset world (remove all objects)
  for (auto &obj : world.objects) {
    if (obj && obj->isAlive) {
      obj->Destroy();
    }
  }
  world.objects.clear();

  // Respawn player
  player = Prefabs::CreatePlayer(world, {0, 0});
  RefreshPlayerLoadout();

  // Set up camera
  CameraManager::Get().SetTarget(&player->position);
  isCameraFollowingPlayer = true;

  // Start stage
  StartStage(0);
}

void GameManager::DevGen() { GenEnemy(10); }

void GameManager::GenEnemy(int count) {
  for (int i = 0; i < count; i++) {
    Vector2 pos =
        MathUtils::GetRandomArroundPosition(player->position, 300.0f, 700.0f);
    GameObject *enemy1 =
        Prefabs::CreateEnemy(world, pos, player, EnemyDefinitions::KNIGHT);
    (void)enemy1;
  }
}

void GameManager::ClearEnemies() {
  for (auto &obj : world.objects) {
    if (obj->layer == Layer::ENEMY) {
      obj->Destroy();
    }
  }
}

void GameManager::StartStage(int stageIndex) {
  int baseStageCount = static_cast<int>(StageDefinitions::ALL.size());

  if (stageIndex < 0) {
    stageIndex = 0;
  }

  // Infinite mode: repeat stages when past ALL.size()
  if (stageIndex < baseStageCount) {
    // Normal stage
    currentStageIndex = stageIndex;
    stageRepeatCount = 0;

    // Reset scaling
    if (stageIndex == 0) {
      currentHealthScale = 1.0f;
      currentSpawnScale = 1.0f;
      currentSpeedScale = 1.0f;
    }
  } else {
    // Infinite mode: use previous stage's scaling parameters
    int prevStageIndex = stageIndex - 1;
    const StageDefinition *prevStage = nullptr;

    if (prevStageIndex < baseStageCount) {
      prevStage = &StageDefinitions::ALL[prevStageIndex];
    } else {
      prevStage = &currentDynamicStage;
    }

    // Find base stage (cycling)
    currentStageIndex = (stageIndex - 1) % baseStageCount;
    stageRepeatCount = (stageIndex - 1) / baseStageCount;

    // Accumulate scaling
    currentHealthScale *= prevStage->healthScaling;
    currentSpawnScale *= prevStage->spawnCountScaling;
    currentSpeedScale *= prevStage->spawnSpeedScaling;

    // Generate dynamic stage
    currentDynamicStage = StageDefinitions::CreateScaledStage(
        currentStageIndex, stageRepeatCount, currentHealthScale,
        currentSpawnScale, currentSpeedScale);
  }

  ClearEnemies();

  // Get current stage definition
  const StageDefinition *stage = GetCurrentStageDefinition();
  if (!stage) {
    return;
  }

  stageTargetSpawnCount =
      MathUtils::GetRandom(stage->totalSpawnMin, stage->totalSpawnMax);
  stageSpawnedCount = 0;
  currentBossCount = 0;       // Reset boss count for new stage
  stageSpawnComplete = false; // Reset spawn completion flag
  gameCleared = false;        // Always false in infinite mode
  stageBannerTimer = kStageBannerDuration;
  nextStageSpawnIn = 0.1f;
}

const StageDefinition *GameManager::GetCurrentStageDefinition() const {
  int baseStageCount = static_cast<int>(StageDefinitions::ALL.size());

  if (currentStageIndex < 0 || currentStageIndex >= baseStageCount) {
    return &currentDynamicStage;
  }

  if (stageRepeatCount > 0) {
    return &currentDynamicStage;
  }

  return &StageDefinitions::ALL[currentStageIndex];
}

void GameManager::SpawnStageEnemy() {
  const StageDefinition *stage = GetCurrentStageDefinition();
  if (!stage || !player || stage->enemies.empty()) {
    return;
  }

  int totalWeight = 0;
  for (const auto &entry : stage->enemies) {
    totalWeight += std::max(0, entry.weight);
  }

  if (totalWeight <= 0) {
    return;
  }

  int roll = MathUtils::GetRandom(1, totalWeight);
  const EnemyDefinition *selectedEnemy = stage->enemies.front().enemy;
  int accum = 0;
  for (const auto &entry : stage->enemies) {
    accum += std::max(0, entry.weight);
    if (roll <= accum) {
      selectedEnemy = entry.enemy;
      break;
    }
  }

  if (!selectedEnemy) {
    return;
  }

  // Limit boss spawns based on maxBossCount
  if (selectedEnemy->isBoss && stage->maxBossCount > 0) {
    // Count current bosses on screen
    int currentBosses = 0;
    for (auto *obj : world.GetObjectsByLayer(Layer::ENEMY)) {
      if (!obj || !obj->isAlive) {
        continue;
      }
      auto health = obj->GetComponent<Health>();
      if (health && health->isBoss) {
        currentBosses++;
      }
    }

    // Only spawn if under the limit
    if (currentBosses >= stage->maxBossCount) {
      // Skip this spawn and try again next time
      return;
    }
  }

  Vector2 pos = MathUtils::GetRandomArroundPosition(
      player->position, stage->spawnRadiusMin, stage->spawnRadiusMax);
  Prefabs::CreateEnemy(world, pos, player, *selectedEnemy, currentHealthScale);
  stageSpawnedCount++;
}

void GameManager::UpdateStage(float dt) {
  const StageDefinition *stage = GetCurrentStageDefinition();
  if (!stage || gameCleared || gameOver) {
    return;
  }

  // No time limit; control progression by spawn count only
  if (!stageSpawnComplete) {
    nextStageSpawnIn -= dt;

    while (stageSpawnedCount < stageTargetSpawnCount &&
           nextStageSpawnIn <= 0.0f) {
      SpawnStageEnemy();
      if (stageSpawnedCount >= stageTargetSpawnCount) {
        stageSpawnComplete = true;
        break;
      }

      float stageProgress = 0.0f;
      if (stageTargetSpawnCount > 0) {
        stageProgress = static_cast<float>(stageSpawnedCount) /
                        static_cast<float>(stageTargetSpawnCount);
      }

      float earlyStageEase = 1.35f - std::min(1.0f, stageProgress) * 0.35f;
      float spawnInterval =
          GetSpawnInterval(*stage, stageTargetSpawnCount, stageSpawnedCount,
                           stageProgress * stage->duration) *
          earlyStageEase;
      nextStageSpawnIn += spawnInterval;
    }
  }

  // Advance when all monsters are spawned and defeated
  if (stageSpawnComplete && world.GetObjectsByLayer(Layer::ENEMY).empty()) {
    StartStage(currentStageIndex + 1);
  }
}

void GameManager::HandleGlobalEvents() {
  // Press Enter to restart on game over
  if (gameOver) {
    if (IsKeyPressed(KEY_ENTER)) {
      // Restart game
      RestartGame();
    }
    return;
  }

  if (IsKeyPressed(KEY_Q)) {
    ToggleShop();
  }

  if (shopOpen) {
    if (IsKeyPressed(KEY_ONE)) {
      PurchaseShopOffer(0);
    }
    if (IsKeyPressed(KEY_TWO)) {
      PurchaseShopOffer(1);
    }
    if (IsKeyPressed(KEY_THREE)) {
      PurchaseShopOffer(2);
    }
    if (IsKeyPressed(KEY_FOUR)) {
      PurchaseShopOffer(3);
    }
    if (IsKeyPressed(KEY_FIVE)) {
      PurchaseShopOffer(4);
    }
    if (IsKeyPressed(KEY_E)) {
      RerollShopOffers();
    }
    return;
  }

  if (progression.damageBurstUnlocked &&
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (progression.damageBurstKeepingCount > 0) {
      // Use keeping count if available
      Vector2 mouseWorldPos = GetScreenToWorld2D(
          GetMousePosition(), CameraManager::Get().GetCamera());
      Prefabs::CreateEffect(world, mouseWorldPos, BuildDamageBurstDefinition());
      progression.damageBurstKeepingCount--;
    } else if (!damageBurstCooldownTimer.IsRunning()) {
      // Use if no keeping count and not on cooldown
      Vector2 mouseWorldPos = GetScreenToWorld2D(
          GetMousePosition(), CameraManager::Get().GetCamera());
      Prefabs::CreateEffect(world, mouseWorldPos, BuildDamageBurstDefinition());
      damageBurstCooldownTimer.Reset(GetDamageBurstCooldown());
    }
  }

  if (progression.damageZoneUnlocked &&
      IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    if (progression.damageZoneKeepingCount > 0) {
      // Use keeping count if available
      Vector2 mouseWorldPos = GetScreenToWorld2D(
          GetMousePosition(), CameraManager::Get().GetCamera());
      Prefabs::CreateEffect(world, mouseWorldPos, BuildDamageZoneDefinition());
      progression.damageZoneKeepingCount--;
    } else if (!damageZoneCooldownTimer.IsRunning()) {
      // Use if no keeping count and not on cooldown
      Vector2 mouseWorldPos = GetScreenToWorld2D(
          GetMousePosition(), CameraManager::Get().GetCamera());
      Prefabs::CreateEffect(world, mouseWorldPos, BuildDamageZoneDefinition());
      damageZoneCooldownTimer.Reset(GetDamageZoneCooldown());
    }
  }

  if (IsKeyPressed(KEY_TAB)) {
    if (isCameraFollowingPlayer) {
      std::vector<GameObject *> enemies = world.GetObjectsByLayer(Layer::ENEMY);
      if (!enemies.empty()) {
        int randomIdx = GetRandomValue(0, enemies.size() - 1);
        CameraManager::Get().SetTarget(&enemies[randomIdx]->position);
        isCameraFollowingPlayer = false;
      }
    } else {
      CameraManager::Get().SetTarget(&player->position);
      isCameraFollowingPlayer = true;
    }
  }

  // R key: collect all coins at once (1-minute cooldown)
  if (IsKeyPressed(KEY_R) && !coinMagnetTimer.IsRunning()) {
    std::vector<GameObject *> items = world.GetObjectsByLayer(Layer::ITEM);
    int collectedCount = 0;
    for (auto *item : items) {
      if (item && item->isAlive) {
        auto magnet = item->GetComponent<Magnet>();
        if (magnet && !magnet->isMagnetizing) {
          magnet->targetPos = &player->position;
          magnet->speed = 240.0f;
          magnet->acceleration = 1400.0f;
          magnet->maxSpeed = 1200.0f;
          collectedCount++;
        }
      }
    }
    if (collectedCount > 0) {
      coinMagnetTimer.Reset(60.0f); // 1-minute cooldown
    }
  }

  if (IsKeyDown(KEY_LEFT)) {
    gameSpeed = std::max(0.1f, gameSpeed - GetFrameTime());
  }

  if (IsKeyDown(KEY_RIGHT)) {
    gameSpeed = std::min(5.0f, gameSpeed + GetFrameTime());
  }

  if (IsKeyPressed(KEY_SPACE)) {
    gameSpeed = 1.0f;
  }
}

void GameManager::AddShock(Vector2 absolutePos) {
  Vector2 screenPosition =
      GetWorldToScreen2D(absolutePos, CameraManager::Get().GetCamera());
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

void GameManager::Update() {
  float dt = GetFrameTime();

  HandleGlobalEvents();

  // Stop updating during game over
  if (gameOver) {
    return;
  }

  float scaledDt = shopOpen ? 0.0f : dt * gameSpeed;
  if (!shopOpen) {
    damageBurstCooldownTimer.Update(dt);
    damageZoneCooldownTimer.Update(dt);
    coinMagnetTimer.Update(dt);

    // Increase keeping count on cooldown complete (max 3)
    if (progression.damageBurstUnlocked &&
        damageBurstCooldownTimer.DidCompleteThisFrame()) {
      progression.damageBurstKeepingCount =
          std::min(3, progression.damageBurstKeepingCount + 1);
    }

    if (progression.damageZoneUnlocked &&
        damageZoneCooldownTimer.DidCompleteThisFrame()) {
      progression.damageZoneKeepingCount =
          std::min(3, progression.damageZoneKeepingCount + 1);
    }
  }
  world.Update(scaledDt);
  UpdateStage(scaledDt);
  stageBannerTimer = std::max(0.0f, stageBannerTimer - dt);

  CameraManager::Get().Update(dt);

  for (int i = 0; i < NUM_SHOCKWAVES; i++) {
    if (shockTimes[i] < 1.0f) {
      shockTimes[i] += dt;
    }
  }

  SetShaderValueV(shader, centresLoc, shockCentres, SHADER_UNIFORM_VEC2,
                  NUM_SHOCKWAVES);
  SetShaderValueV(shader, timesLoc, shockTimes, SHADER_UNIFORM_FLOAT,
                  NUM_SHOCKWAVES);
}

void GameManager::DrawUI() {
  // Game over screen
  if (gameOver) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

    const char *gameOverText = "GAME OVER";
    int fontSize = 80;
    int textWidth = MeasureText(gameOverText, fontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    int y = (GetScreenHeight() - fontSize) / 2 - 50;

    DrawText(gameOverText, x + 4, y + 4, fontSize, Fade(BLACK, 0.5f));
    DrawText(gameOverText, x, y, fontSize, RED);

    const char *restartText = "Press ENTER to Restart";
    int restartFontSize = 30;
    int restartTextWidth = MeasureText(restartText, restartFontSize);
    int restartX = (GetScreenWidth() - restartTextWidth) / 2;
    int restartY = y + fontSize + 30;

    DrawText(restartText, restartX + 2, restartY + 2, restartFontSize,
             Fade(BLACK, 0.5f));
    DrawText(restartText, restartX, restartY, restartFontSize, WHITE);

    return;
  }

  // Player HP bar (top center of screen)
  if (player) {
    auto playerHealth = player->GetComponent<Health>();
    if (playerHealth) {
      float barWidth = 300.0f;
      float barHeight = 20.0f;
      float barX = (GetScreenWidth() - barWidth) / 2.0f;
      float barY = 20.0f;
      float hpPercent = playerHealth->hp / playerHealth->maxHp;

      // Background (dark gray)
      DrawRectangleRec({barX, barY, barWidth, barHeight}, DARKGRAY);

      // HP bar (green -> orange -> red gradient)
      Color hpColor = hpPercent > 0.5f    ? GREEN
                      : hpPercent > 0.25f ? ORANGE
                                          : RED;
      DrawRectangleRec({barX, barY, barWidth * hpPercent, barHeight}, hpColor);

      // Border
      DrawRectangleLinesEx({barX, barY, barWidth, barHeight}, 2.0f, BLACK);

      // HP text (below the bar)
      const char *hpText =
          TextFormat("%d/%d", (int)playerHealth->hp, (int)playerHealth->maxHp);
      int textWidth = MeasureText(hpText, 18);
      DrawText(hpText, (GetScreenWidth() - textWidth) / 2, barY + barHeight + 5,
               18, WHITE);
    }
  }

  DrawText("Use WASD to move", 10, 30, 20, DARKGRAY);
  DrawText("Tab: Switch Camera", 10, 50, 20, DARKGRAY);
  DrawText("Q: Toggle Shop", 10, 70, 20, DARKBLUE);

  // R key coin collection status
  if (coinMagnetTimer.IsRunning()) {
    float remainingTime = 60.0f - coinMagnetTimer.GetCurrentTime();
    DrawText(TextFormat("R: Coin Magnet %.0fs", remainingTime), 10, 90, 20,
             GRAY);
  } else {
    DrawText("R: Collect All Coins", 10, 90, 20, GOLD);
  }

  for (auto &obj : world.objects) {
    obj->DrawUI();
  }

  DrawText(TextFormat("Coins: %d", coin), 10, 110, 20, GOLD);
  DrawText(TextFormat("xSpeed %.1f", gameSpeed), 10, 130, 20, DARKGREEN);

  // enemy count
  int count = 0;
  std::vector<GameObject *> enemies = world.GetObjectsByLayer(Layer::ENEMY);
  count = enemies.size();
  const char *enemyText = TextFormat("Enemies: %d", count);
  int enemyTextWidth = MeasureText(enemyText, 20);
  DrawText(enemyText, GetScreenWidth() - enemyTextWidth - 16, 110, 20, RED);

  const char *stageText = gameCleared ? "Stage Clear" : "Stage --";
  if (const StageDefinition *stage = GetCurrentStageDefinition()) {
    stageText = stage->name.c_str();
  }

  int fontSize = 24;
  int textWidth = MeasureText(stageText, fontSize);
  DrawText(stageText, GetScreenWidth() - textWidth - 16, 16, fontSize, BLACK);

  if (const StageDefinition *stage = GetCurrentStageDefinition()) {
    const char *spawnText;
    if (stageSpawnComplete) {
      spawnText = "Spawning Complete";
    } else {
      spawnText =
          TextFormat("Spawn %d/%d", stageSpawnedCount, stageTargetSpawnCount);
    }
    int spawnTextWidth = MeasureText(spawnText, 20);
    DrawText(spawnText, GetScreenWidth() - spawnTextWidth - 16, 44, 20,
             stageSpawnComplete ? GREEN : DARKGRAY);
  }

  if (stageBannerTimer > 0.0f && !gameCleared) {
    const StageDefinition *stage = GetCurrentStageDefinition();
    if (stage) {
      float progress = 1.0f - (stageBannerTimer / kStageBannerDuration);
      float fade = 1.0f - progress;
      int fontSize = 64;
      const char *bannerText = stage->name.c_str();
      int bannerWidth = MeasureText(bannerText, fontSize);
      int x = (GetScreenWidth() - bannerWidth) / 2;
      int y = (GetScreenHeight() - fontSize) / 2;
      DrawText(bannerText, x + 3, y + 3, fontSize, Fade(BLACK, fade * 0.35f));
      DrawText(bannerText, x, y, fontSize, Fade(WHITE, fade));
    }
  }

  float skillBaseY = static_cast<float>(GetScreenHeight()) - 72.0f;
  if (progression.damageBurstUnlocked) {
    const char *burstState;
    if (progression.damageBurstKeepingCount > 0) {
      // Show keeping count priority
      if (damageBurstCooldownTimer.IsRunning()) {
        float remainingTime = GetDamageBurstCooldown() -
                              damageBurstCooldownTimer.GetCurrentTime();
        burstState =
            TextFormat("LMB Burst [%d/3] %.1fs",
                       progression.damageBurstKeepingCount, remainingTime);
      } else {
        burstState =
            TextFormat("LMB Burst [%d/3]", progression.damageBurstKeepingCount);
      }
    } else {
      // Show only cooldown state if no keeping count
      burstState =
          damageBurstCooldownTimer.IsRunning()
              ? TextFormat("LMB Burst %.1fs",
                           GetDamageBurstCooldown() -
                               damageBurstCooldownTimer.GetCurrentTime())
              : "LMB Burst Ready";
    }
    DrawText(burstState, 14, static_cast<int>(skillBaseY), 20,
             (progression.damageBurstKeepingCount == 0 &&
              damageBurstCooldownTimer.IsRunning())
                 ? GRAY
                 : SKYBLUE);
  } else {
    DrawText("LMB Burst Locked", 14, static_cast<int>(skillBaseY), 20, GRAY);
  }

  if (progression.damageZoneUnlocked) {
    const char *zoneState;
    if (progression.damageZoneKeepingCount > 0) {
      // Show keeping count priority
      if (damageZoneCooldownTimer.IsRunning()) {
        float remainingTime =
            GetDamageZoneCooldown() - damageZoneCooldownTimer.GetCurrentTime();
        zoneState =
            TextFormat("RMB Gas [%d/3] %.1fs",
                       progression.damageZoneKeepingCount, remainingTime);
      } else {
        zoneState =
            TextFormat("RMB Gas [%d/3]", progression.damageZoneKeepingCount);
      }
    } else {
      // Show only cooldown state if no keeping count
      zoneState = damageZoneCooldownTimer.IsRunning()
                      ? TextFormat("RMB Gas %.1fs",
                                   GetDamageZoneCooldown() -
                                       damageZoneCooldownTimer.GetCurrentTime())
                      : "RMB Gas Ready";
    }
    DrawText(zoneState, 14, static_cast<int>(skillBaseY + 24.0f), 20,
             (progression.damageZoneKeepingCount == 0 &&
              damageZoneCooldownTimer.IsRunning())
                 ? GRAY
                 : GREEN);
  } else {
    DrawText("RMB Gas Locked", 14, static_cast<int>(skillBaseY + 24.0f), 20,
             GRAY);
  }

  if (shopOpen) {
    DrawShopOverlay();
  }
}

void GameManager::Draw() {
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
  Rectangle sourceRec = {0, 0, static_cast<float>(renderTexture.texture.width),
                         static_cast<float>(-renderTexture.texture.height)};
  DrawTextureRec(renderTexture.texture, sourceRec, {0, 0}, WHITE);
  EndShaderMode();

  DrawUI();
  DrawFPS(10, 10);

  EndDrawing();
}

void GameManager::Clear() {
  if (shader.id != 0) {
    UnloadShader(shader);
    shader.id = 0;
  }
}

void GameManager::AddCoin(int amount) { coin += amount; }
