#pragma once

#include "Component.hpp"
#include "Event.hpp"
#include "raylib.h"

class World;

enum class ColliderType { BOX, CIRCLE };

struct CollisionRatios {
  float a;
  float b;
};

class Collider : public Component {
public:
  inline static bool debugMode = false;

  ColliderType type;
  Vector2 offset = {0, 0};
  bool isStatic = false;
  bool isTrigger = false;
  float mass = 1.0f;

  Event<Collider *> onTriggerEnter;

  Collider(ColliderType type, Vector2 offset) : type(type), offset(offset) {}
  ~Collider() override = default;

  void Start() override;
  void OnDestroy() override;

  static CollisionRatios GetResponseRatios(Collider *a, Collider *b) {
    if (a->isTrigger || b->isTrigger) {
      return {0.0f, 0.0f};
    };

    if (a->isStatic && b->isStatic)
      return {0.0f, 0.0f};
    if (a->isStatic)
      return {0.0f, 1.0f};
    if (b->isStatic)
      return {1.0f, 0.0f};

    float totalMass = a->mass + b->mass;
    if (totalMass > 0) {
      return {b->mass / totalMass, a->mass / totalMass};
    }

    return {
        0.5f,
        0.5f,
    };
  }
};
