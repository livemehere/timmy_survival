#include "World.hpp"
#include "Components/BoxCollider.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

GameObject *World::CreateObject(std::string name) {
  auto obj = std::make_shared<GameObject>(name, this);
  objects.push_back(obj);
  return obj.get();
}

void World::Update(float dt) {
  for (auto &obj : objects) {
    obj->Update(dt);
  }
}

void World::Draw() {
  for (auto &obj : objects) {
    obj->Draw();
  }
}

std::vector<BoxCollider *> World::GetAllColliders() {
  std::vector<BoxCollider *> colliders;
  for (auto &obj : objects) {
    auto col = obj->GetComponent<BoxCollider>();
    if (col)
      colliders.push_back(col);
  }
  return colliders;
}

void World::ResolveCollisions() {
  auto colliders = GetAllColliders();

  for (size_t i = 0; i < colliders.size(); i++) {
    for (size_t j = i + 1; j < colliders.size(); j++) {
      BoxCollider *a = colliders[i];
      BoxCollider *b = colliders[j];

      Rectangle rectA = a->GetRect();
      Rectangle rectB = b->GetRect();

      if (CheckCollisionRecs(rectA, rectB)) {
        Vector2 ca = {rectA.x + rectA.width / 2, rectA.y + rectA.height / 2};
        Vector2 cb = {rectB.x + rectB.width / 2, rectB.y + rectB.height / 2};

        // Vector2 minDist =
        // Vector2 distance = Vector2Subtract(ca, cb);
        // Vector2 pushDir = Vector2Normalize(overlap);
      }
    }
  }
}
