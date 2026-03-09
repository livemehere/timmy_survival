#pragma once

#include <deque>
#include <iostream>
#include <unordered_set>

#include "../Components/Health.hpp"
#include "../GameObject.hpp"
#include "../Timer.hpp"
#include "../World.hpp"
#include "CircleCollider.hpp"
#include "Collider.hpp"
#include "Knockback.hpp"
#include "SpriteRenderer.hpp"
#include "raylib.h"

class Projectile : public Component {
public:
  Timer lifeTimer;
  float damage;
  int pierce = 1;
  float knockbackForce = 150.0f;

  std::unordered_set<GameObject *> hitObjects;

  // effects
  std::deque<Vector2> history;
  Color color = LIME;
  int maxHistory = 10;
  float tailTimer = 0.0f;
  float tailDelay = 0.05f;

  Projectile(float damage, float lifetime, int pierce = 1)
      : lifeTimer(lifetime, false), damage(damage), pierce(pierce) {}

  void Update(float dt) override {
    if (lifeTimer.Update(dt)) {
      gameObject->Destroy();
    }

    // tail effect
    tailTimer += dt;
    if (tailTimer >= tailDelay) {
      tailTimer -= tailDelay;
      history.push_front(gameObject->position);

      if (history.size() > maxHistory) {
        history.pop_back();
      }
    }
  }

  void OnTriggerEnter(Collider *other) override {

    if (other->gameObject->layer != Layer::ENEMY)
      return;

    if (hitObjects.find(other->gameObject) != hitObjects.end())
      return;

    auto health = other->gameObject->GetComponent<Health>();
    if (health) {

      hitObjects.insert(other->gameObject);

      pierce--;
      health->TakeDamage(damage);

      auto knockback = other->gameObject->GetComponent<Knockback>();
      if (knockback) {
        Vector2 dir =
            Vector2Subtract(other->gameObject->position, gameObject->position);
        dir = Vector2Normalize(dir);
        knockback->Apply(Vector2Scale(dir, knockbackForce));
      }

      gameObject->world->cm->Shake(1.0f, 0.1f);
    }

    if (pierce <= 0) {
      gameObject->Destroy();
    }
  }

  void Draw() override {
    BeginBlendMode(BLEND_ADDITIVE);
    int size = history.size();
    auto collider = gameObject->GetComponent<CircleCollider>();
    Vector2 prevPos = gameObject->position;

    for (size_t i = 0; i < size; i++) {
      Vector2 pos = history[i];
      float radiusRatio = (float)(size - i) / size;
      float radius = collider->radius * radiusRatio;

      DrawLineEx(prevPos, pos, radius, Fade(color, radiusRatio));

      prevPos = pos;
    }
    EndBlendMode();
  }
};
