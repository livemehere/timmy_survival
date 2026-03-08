#include "World.hpp"
#include "Components/Collider.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/CircleCollider.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

// TODO: improve performance to use. this is simple implementation.
GameObject *World::GetObjectByName(std::string name) {
  for (auto &obj : objects) {
    if (obj->name == name) {
      return obj.get();
    }
  }
  return nullptr;
}

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
  Vector2 ca = a->GetCenter();
  Vector2 cb = b->GetCenter();
  float cr = a->radius;
  float rb = b->radius;

  if (CheckCollisionCircles(ca, cr, cb, rb)) {
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
