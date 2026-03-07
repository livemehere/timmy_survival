#include "World.hpp"
#include "Collider.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/CircleCollider.hpp"
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

std::vector<Collider *> World::GetAllColliders() {
  std::vector<Collider *> colliders;
  for (auto &obj : objects) {
    auto col = obj->GetComponent<Collider>();
    if (col)
      colliders.push_back(col);
  }
  return colliders;
}

void World::ResolveCollisions() {
  auto colliders = GetAllColliders();

  for (size_t i = 0; i < colliders.size(); i++) {
    for (size_t j = i + 1; j < colliders.size(); j++) {
      Collider *a = colliders[i];
      Collider *b = colliders[j];

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
}
void World::ResolveBoxBoxCollision(BoxCollider *a, BoxCollider *b) {}
void World::ResolveCircleBoxCollision(CircleCollider *circle,
                                      BoxCollider *box) {}
