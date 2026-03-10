#include "World.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/CircleCollider.hpp"
#include "Components/Collider.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cstddef>

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
  const size_t updateCount = objects.size();
  for (size_t i = 0; i < updateCount; i++) {
    objects[i]->Update(dt);
  }

  objects.erase(std::remove_if(objects.begin(), objects.end(),
                               [](const std::shared_ptr<GameObject> &obj) {
                                 return !obj->isAlive;
                               }),
                objects.end());
}

void World::DrawBg() {
  static int gridSize = 64;
  Camera2D camera = cm->GetCamera();

  Vector2 topLeft = GetScreenToWorld2D({0.0f, 0.0f}, camera);
  Vector2 bottomRight = GetScreenToWorld2D(
      {(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);

  float startX = std::floor(topLeft.x / gridSize) * gridSize;
  float startY = std::floor(topLeft.y / gridSize) * gridSize;

  for (float x = startX; x <= bottomRight.x + gridSize; x += gridSize) {
    DrawLineV({x, topLeft.y - gridSize}, {x, bottomRight.y + gridSize},
              Fade(GRAY, 0.08f));
  }

  for (float y = startY; y <= bottomRight.y + gridSize; y += gridSize) {
    DrawLineV({topLeft.x - gridSize, y}, {bottomRight.x + gridSize, y},
              Fade(GRAY, 0.08f));
  }
}

void World::Draw() {
  DrawBg();
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
}
