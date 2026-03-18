#include "World.hpp"
#include "../Components/Colliders/BoxCollider.hpp"
#include "../Components/Colliders/CircleCollider.hpp"
#include "../Managers/CameraManager.hpp"
#include "Collider.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <unordered_map>

namespace {
constexpr float kMapHalfSize = 960.0f;
constexpr float kTileSize = 64.0f;
constexpr float kCollisionCellSize = 96.0f;
constexpr Color kGrassA = {110, 156, 84, 255};
constexpr Color kGrassB = {95, 142, 72, 255};
constexpr Color kPathTint = {136, 122, 88, 255};
constexpr Color kBushColor = {67, 112, 48, 255};
constexpr Color kBushShadow = {42, 72, 28, 180};
constexpr Color kTreeLeaf = {48, 102, 52, 255};
constexpr Color kTreeLeafLight = {82, 132, 73, 220};
constexpr Color kTreeTrunk = {88, 60, 39, 255};
constexpr Color kWallColor = {79, 103, 70, 255};
constexpr Color kWallHighlight = {123, 150, 107, 255};
constexpr Color kFogColor = {31, 56, 34, 210};

float HashCoord(int x, int y) {
  float v = std::sin(x * 12.9898f + y * 78.233f) * 43758.5453f;
  return v - std::floor(v);
}

int GetCollisionRadius(Collider *collider) {
  if (!collider) {
    return 0;
  }

  if (collider->type == ColliderType::CIRCLE) {
    return static_cast<int>(
        std::ceil(static_cast<CircleCollider *>(collider)->radius));
  }

  Rectangle rect = static_cast<BoxCollider *>(collider)->GetRect();
  return static_cast<int>(std::ceil(std::max(rect.width, rect.height) * 0.5f));
}

Vector2 GetColliderCenter(Collider *collider) {
  if (collider->type == ColliderType::CIRCLE) {
    return static_cast<CircleCollider *>(collider)->GetCenter();
  }

  Rectangle rect = static_cast<BoxCollider *>(collider)->GetRect();
  return {rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f};
}

long long MakeCellKey(int x, int y) {
  return (static_cast<long long>(x) << 32) ^ static_cast<unsigned int>(y);
}

bool ShouldCheckPair(Collider *a, Collider *b) {
  if (!a || !b || !a->gameObject || !b->gameObject ||
      !a->gameObject->isAlive || !b->gameObject->isAlive || a == b) {
    return false;
  }

  if (a->type == ColliderType::CIRCLE && b->type == ColliderType::CIRCLE) {
    Vector2 ca = static_cast<CircleCollider *>(a)->GetCenter();
    Vector2 cb = static_cast<CircleCollider *>(b)->GetCenter();
    float radiusSum = static_cast<CircleCollider *>(a)->radius +
                      static_cast<CircleCollider *>(b)->radius;
    return Vector2DistanceSqr(ca, cb) <= radiusSum * radiusSum;
  }

  if (a->type == ColliderType::BOX && b->type == ColliderType::BOX) {
    return CheckCollisionRecs(static_cast<BoxCollider *>(a)->GetRect(),
                              static_cast<BoxCollider *>(b)->GetRect());
  }

  CircleCollider *circle = a->type == ColliderType::CIRCLE
                               ? static_cast<CircleCollider *>(a)
                               : static_cast<CircleCollider *>(b);
  BoxCollider *box = a->type == ColliderType::BOX ? static_cast<BoxCollider *>(a)
                                                  : static_cast<BoxCollider *>(b);
  Vector2 center = circle->GetCenter();
  Rectangle rect = box->GetRect();
  float closestX = std::clamp(center.x, rect.x, rect.x + rect.width);
  float closestY = std::clamp(center.y, rect.y, rect.y + rect.height);
  return Vector2DistanceSqr(center, {closestX, closestY}) <=
         circle->radius * circle->radius;
}

void DrawBushCluster(Vector2 center, float radius) {
  DrawCircleV({center.x + 4.0f, center.y + 6.0f}, radius * 1.05f, kBushShadow);
  DrawCircleV({center.x - radius * 0.35f, center.y}, radius, kBushColor);
  DrawCircleV({center.x + radius * 0.4f, center.y + 2.0f}, radius * 0.85f, Fade(kBushColor, 0.92f));
  DrawCircleV({center.x, center.y - radius * 0.35f}, radius * 0.78f, Fade(kGrassA, 0.65f));
}

void DrawTree(Vector2 center, float scale) {
  Rectangle trunk = {center.x - 7.0f * scale, center.y + 10.0f * scale,
                     14.0f * scale, 34.0f * scale};
  DrawRectangleRounded(trunk, 0.35f, 4, kTreeTrunk);
  DrawCircleV({center.x, center.y}, 24.0f * scale, kTreeLeaf);
  DrawCircleV({center.x - 18.0f * scale, center.y + 4.0f * scale},
              17.0f * scale, Fade(kTreeLeafLight, 0.95f));
  DrawCircleV({center.x + 18.0f * scale, center.y + 2.0f * scale},
              16.0f * scale, Fade(kTreeLeaf, 0.92f));
  DrawCircleV({center.x, center.y - 12.0f * scale}, 15.0f * scale,
              Fade(kTreeLeafLight, 0.78f));
}

void ClampDynamicObject(GameObject *obj) {
  if (!obj || !obj->isAlive) {
    return;
  }

  if (obj->layer != Layer::PLAYER && obj->layer != Layer::ENEMY &&
      obj->layer != Layer::ITEM) {
    return;
  }

  obj->position.x =
      std::clamp(obj->position.x, -kMapHalfSize, kMapHalfSize);
  obj->position.y =
      std::clamp(obj->position.y, -kMapHalfSize, kMapHalfSize);
}
} // namespace

GameObject *World::CreateObject(std::string name) {
  auto obj = std::make_unique<GameObject>(name, this);
  GameObject *ptr = obj.get();
  objects.push_back(std::move(obj));
  return ptr;
}

void World::Update(float dt) {
  const size_t updateCount = objects.size();
  for (size_t i = 0; i < updateCount; i++) {
    objects[i]->Update(dt);
    ClampDynamicObject(objects[i].get());
  }

  ResolveCollisions();

  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [](const std::unique_ptr<GameObject> &obj) {
                                 if (!obj->isAlive) {
                                   obj->OnDestroy();
                                   return true;
                                 }
                                 return false;
                               }),
                objects.end());
}

void World::DrawBg() {
  Camera2D camera = CameraManager::Get().GetCamera();

  Vector2 topLeft = GetScreenToWorld2D({0.0f, 0.0f}, camera);
  Vector2 bottomRight = GetScreenToWorld2D(
      {(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);

  float minX = -kMapHalfSize;
  float maxX = kMapHalfSize;
  float minY = -kMapHalfSize;
  float maxY = kMapHalfSize;

  DrawRectangle(static_cast<int>(minX), static_cast<int>(minY),
                static_cast<int>(maxX - minX), static_cast<int>(maxY - minY),
                kGrassA);

  int startTileX =
      static_cast<int>(std::floor(std::max(topLeft.x, minX) / kTileSize)) - 1;
  int endTileX =
      static_cast<int>(std::ceil(std::min(bottomRight.x, maxX) / kTileSize)) + 1;
  int startTileY =
      static_cast<int>(std::floor(std::max(topLeft.y, minY) / kTileSize)) - 1;
  int endTileY =
      static_cast<int>(std::ceil(std::min(bottomRight.y, maxY) / kTileSize)) + 1;

  for (int tx = startTileX; tx <= endTileX; tx++) {
    for (int ty = startTileY; ty <= endTileY; ty++) {
      float worldX = tx * kTileSize;
      float worldY = ty * kTileSize;
      if (worldX > maxX || worldX + kTileSize < minX || worldY > maxY ||
          worldY + kTileSize < minY) {
        continue;
      }

      float noise = HashCoord(tx, ty);
      Color tileColor = noise > 0.5f ? kGrassA : kGrassB;
      DrawRectangle(static_cast<int>(worldX), static_cast<int>(worldY),
                    static_cast<int>(kTileSize + 1.0f),
                    static_cast<int>(kTileSize + 1.0f), tileColor);

      if (noise > 0.76f) {
        DrawCircleV({worldX + 14.0f + HashCoord(tx + 3, ty) * 20.0f,
                     worldY + 12.0f + HashCoord(tx, ty + 7) * 22.0f},
                    3.0f + HashCoord(tx + 1, ty + 5) * 3.0f,
                    Fade(kGrassB, 0.55f));
      }
    }
  }

  DrawRectangleRounded({-84.0f, minY, 168.0f, maxY - minY}, 0.18f, 8,
                       Fade(kPathTint, 0.65f));
  DrawRectangleRounded({minX, -72.0f, maxX - minX, 144.0f}, 0.18f, 8,
                       Fade(kPathTint, 0.45f));

  for (int i = -14; i <= 14; i++) {
    float x = i * 68.0f;
    if (std::abs(x) < 180.0f) {
      continue;
    }
    DrawBushCluster({x, minY + 54.0f + HashCoord(i, 2) * 20.0f},
                    18.0f + HashCoord(i, 3) * 8.0f);
    DrawBushCluster({x, maxY - 60.0f - HashCoord(i, 4) * 18.0f},
                    18.0f + HashCoord(i, 5) * 8.0f);
  }

  for (int i = -13; i <= 13; i++) {
    float y = i * 72.0f;
    if (std::abs(y) < 160.0f) {
      continue;
    }
    DrawBushCluster({minX + 56.0f + HashCoord(7, i) * 18.0f, y},
                    17.0f + HashCoord(8, i) * 7.0f);
    DrawBushCluster({maxX - 58.0f - HashCoord(9, i) * 18.0f, y},
                    17.0f + HashCoord(10, i) * 7.0f);
  }

  for (int i = -8; i <= 8; i++) {
    float offset = i * 118.0f;
    if (std::abs(offset) < 220.0f) {
      continue;
    }
    DrawTree({minX + 118.0f + HashCoord(i, 11) * 36.0f,
              offset + HashCoord(i, 12) * 36.0f},
             0.9f + HashCoord(i, 13) * 0.35f);
    DrawTree({maxX - 120.0f - HashCoord(i, 14) * 32.0f,
              offset + HashCoord(i, 15) * 28.0f},
             0.92f + HashCoord(i, 16) * 0.3f);
    DrawTree({offset + HashCoord(i, 17) * 34.0f,
              minY + 122.0f + HashCoord(i, 18) * 28.0f},
             0.88f + HashCoord(i, 19) * 0.32f);
    DrawTree({offset + HashCoord(i, 20) * 38.0f,
              maxY - 124.0f - HashCoord(i, 21) * 26.0f},
             0.9f + HashCoord(i, 22) * 0.34f);
  }

  DrawRectangleLinesEx({minX, minY, maxX - minX, maxY - minY}, 18.0f,
                       Fade(kWallColor, 0.95f));
  DrawRectangleLinesEx({minX + 8.0f, minY + 8.0f, maxX - minX - 16.0f,
                        maxY - minY - 16.0f},
                       3.0f, Fade(kWallHighlight, 0.85f));

  DrawRectangle(static_cast<int>(minX - 600.0f), static_cast<int>(minY - 600.0f),
                static_cast<int>(maxX - minX + 1200.0f), 560, kFogColor);
  DrawRectangle(static_cast<int>(minX - 600.0f), static_cast<int>(maxY + 40.0f),
                static_cast<int>(maxX - minX + 1200.0f), 560, kFogColor);
  DrawRectangle(static_cast<int>(minX - 600.0f), static_cast<int>(minY), 560,
                static_cast<int>(maxY - minY), kFogColor);
  DrawRectangle(static_cast<int>(maxX + 40.0f), static_cast<int>(minY), 560,
                static_cast<int>(maxY - minY), kFogColor);
}

// affects by camera
void World::Draw() {
  DrawBg();
  for (auto &obj : objects) {
    obj->Draw();
  }
}

// not affects by camera (UI & Text)
void World::DrawUI() {
  for (auto &obj : objects) {
    obj->DrawUI();
  }
}

GameObject *World::GetObjectByName(std::string name) {
  for (auto &obj : objects) {
    if (obj->name == name) {
      return obj.get();
    }
  }

  return nullptr;
}

std::vector<GameObject *> World::GetObjectsByLayer(Layer layer) {
  std::vector<GameObject *> result;
  for (auto &obj : objects) {
    if (obj->layer == layer) {
      result.push_back(obj.get());
    }
  }

  return result;
}

void World::RegisterCollider(Collider *col) { activeColliders.push_back(col); }

void World::UnregisterCollider(Collider *col) {
  activeColliders.erase(
      std::remove(activeColliders.begin(), activeColliders.end(), col),
      activeColliders.end());
}

void World::ResolveCollisions() {
  std::unordered_map<long long, std::vector<Collider *>> grid;
  grid.reserve(activeColliders.size() * 2);

  for (Collider *collider : activeColliders) {
    if (!collider || !collider->gameObject || !collider->gameObject->isAlive) {
      continue;
    }

    Vector2 center = GetColliderCenter(collider);
    int radius = GetCollisionRadius(collider);
    int minCellX =
        static_cast<int>(std::floor((center.x - radius) / kCollisionCellSize));
    int maxCellX =
        static_cast<int>(std::floor((center.x + radius) / kCollisionCellSize));
    int minCellY =
        static_cast<int>(std::floor((center.y - radius) / kCollisionCellSize));
    int maxCellY =
        static_cast<int>(std::floor((center.y + radius) / kCollisionCellSize));

    for (int x = minCellX; x <= maxCellX; x++) {
      for (int y = minCellY; y <= maxCellY; y++) {
        grid[MakeCellKey(x, y)].push_back(collider);
      }
    }
  }

  std::unordered_map<Collider *, std::vector<Collider *>> candidates;
  candidates.reserve(activeColliders.size());

  for (auto &[key, cellColliders] : grid) {
    (void)key;
    for (size_t i = 0; i < cellColliders.size(); i++) {
      for (size_t j = i + 1; j < cellColliders.size(); j++) {
        Collider *a = cellColliders[i];
        Collider *b = cellColliders[j];
        if (!ShouldCheckPair(a, b)) {
          continue;
        }

        if (a > b) {
          std::swap(a, b);
        }
        candidates[a].push_back(b);
      }
    }
  }

  for (auto &[a, list] : candidates) {
    std::sort(list.begin(), list.end());
    list.erase(std::unique(list.begin(), list.end()), list.end());

    for (Collider *b : list) {

      if (a->type == ColliderType::CIRCLE && b->type == ColliderType::CIRCLE) {
        ResolveCircleCircleCollision(static_cast<CircleCollider *>(a),
                                     static_cast<CircleCollider *>(b));
      } else if (a->type == ColliderType::BOX && b->type == ColliderType::BOX) {
        ResolveBoxBoxCollision(static_cast<BoxCollider *>(a),
                               static_cast<BoxCollider *>(b));
      } else {
        // mix case
        if (a->type == ColliderType::CIRCLE) {
          ResolveCircleBoxCollision(static_cast<CircleCollider *>(a),
                                    static_cast<BoxCollider *>(b));
        } else {
          ResolveCircleBoxCollision(static_cast<CircleCollider *>(b),
                                    static_cast<BoxCollider *>(a));
        }
      }
    }
  }
}

void World::ResolveCircleCircleCollision(CircleCollider *a, CircleCollider *b) {
  Vector2 ca = a->GetCenter();
  Vector2 cb = b->GetCenter();
  float cr = a->radius;
  float rb = b->radius;

  if (CheckCollisionCircles(ca, cr, cb, rb)) {

    if (a->isTrigger || b->isTrigger) {
      HandleTriggerEvent(a, b);
      return;
    }

    float dist = Vector2Distance(ca, cb);
    float minDist = cr + rb;
    float overlap = minDist - dist;

    Vector2 pushDir = Vector2Normalize(Vector2Subtract(ca, cb));

    auto [ra, rb] = Collider::GetResponseRatios(a, b);

    a->gameObject->position = Vector2Add(a->gameObject->position,
                                         Vector2Scale(pushDir, overlap * ra));

    b->gameObject->position = Vector2Subtract(
        b->gameObject->position, Vector2Scale(pushDir, overlap * rb));
  };
}
void World::ResolveBoxBoxCollision(BoxCollider *a, BoxCollider *b) {
  Rectangle rectA = a->GetRect();
  Rectangle rectB = b->GetRect();

  if (CheckCollisionRecs(rectA, rectB)) {

    if (a->isTrigger || b->isTrigger) {
      HandleTriggerEvent(a, b);
      return;
    }

    Rectangle overlap = GetCollisionRec(rectA, rectB);
    auto [ra, rb] = Collider::GetResponseRatios(a, b);
    if (overlap.width > overlap.height) {
      float dir = (rectA.x < rectB.x) ? -1.0f : 1.0f;
      a->gameObject->position.x += dir * overlap.width * ra;
      b->gameObject->position.x -= dir * overlap.width * rb;
    } else {
      float dir = (rectA.y < rectB.y) ? -1.0f : 1.0f;
      a->gameObject->position.y += dir * overlap.height * ra;
      b->gameObject->position.y -= dir * overlap.height * rb;
    }
  }
}
void World::ResolveCircleBoxCollision(CircleCollider *circle,
                                      BoxCollider *box) {
  Vector2 center = circle->GetCenter();
  Rectangle rect = box->GetRect();

  float closetX = std::clamp(center.x, rect.x, rect.x + rect.width);
  float closetY = std::clamp(center.y, rect.y, rect.y + rect.height);

  float dist = Vector2Distance(center, {closetX, closetY});
  float minDist = circle->radius;

  if (dist < minDist) {

    if (circle->isTrigger || box->isTrigger) {
      HandleTriggerEvent(circle, box);
      return;
    }

    float overlap = minDist - dist;
    auto [ra, rb] = Collider::GetResponseRatios(circle, box);

    Vector2 pushDir =
        Vector2Normalize(Vector2Subtract(center, {closetX, closetY}));

    circle->gameObject->position = Vector2Add(
        circle->gameObject->position, Vector2Scale(pushDir, overlap * ra));

    box->gameObject->position = Vector2Subtract(
        box->gameObject->position, Vector2Scale(pushDir, overlap * rb));
  }
}

void World::HandleTriggerEvent(Collider *a, Collider *b) {
  for (auto &comp : a->gameObject->components) {
    comp->OnTriggerEnter(b);
  }

  for (auto &comp : b->gameObject->components) {
    comp->OnTriggerEnter(a);
  }

  a->onTriggerEnter.Invoke(b);
  b->onTriggerEnter.Invoke(a);
}
