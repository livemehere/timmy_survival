#pragma once

#include "../GameObject.hpp"
#include "raylib.h"
#include <functional>

class Health : public Component {
public:
  float maxHp;
  float hp;

  float invincibilityTime = 0.1f;
  float invincibilityTimer = 0.0f;

  std::function<void()> onDeath;

  Health(float maxHp) : maxHp(maxHp), hp(maxHp) {}

  void Update(float dt) override {
    if (invincibilityTimer > 0.0f) {
      invincibilityTimer -= dt;
    }
  }

  void TakeDamage(float damage) {
    if (invincibilityTimer > 0.0f || hp <= 0.0f)
      return;

    hp -= damage;
    invincibilityTimer = invincibilityTime;

    if (hp <= 0.0f) {
      hp = 0.0f;
      Die();
    }
  }

  void Die() {
    if (onDeath) {
      onDeath();
    }
    gameObject->Destroy();
  }

  void Draw() override {
    Vector2 pos = gameObject->position;

    float barW = 20.0f;
    float barH = 3.0f;
    float yOffset = 15.0f;
    float hpRatio = hp / maxHp;

    Rectangle bgRec = {pos.x - barW / 2.0f, pos.y - yOffset, barW, barH};

    Rectangle fgRec = {pos.x - barW / 2.0f, pos.y - yOffset, barW * hpRatio,
                       barH};

    DrawRectangleRec(bgRec, DARKGRAY);
    DrawRectangleRec(fgRec, MAROON);
    DrawRectangleLinesEx(bgRec, 0.5f, BLACK);
  }
};
