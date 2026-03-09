#pragma once

#include "../../Utils/MathUtils.hpp"
#include "../GameObject.hpp"
#include "../Timer.hpp"
#include "SpriteRenderer.hpp"
#include "raylib.h"
#include <cmath>
#include <functional>

class Health : public Component {
public:
  float maxHp;
  float hp;

  float invincibilityTime = 0.1f;
  float invincibilityTimer = 0.0f;

  std::function<void()> onDeath;

  float bounceScale = 0.15f;
  Timer bounceTimer = Timer(0.0f, false);
  float squashDirX = 1.0f;
  float squashDirY = -1.0f;

  Health(float maxHp) : maxHp(maxHp), hp(maxHp) {}

  void Update(float dt) override {
    if (invincibilityTimer > 0.0f) {
      invincibilityTimer -= dt;
    }

    if (!bounceTimer.Update(dt)) {
      auto sprite = gameObject->GetComponent<SpriteRenderer>();
      float p = 1.0f - (bounceTimer.currentTime / bounceTimer.targetTime);
      float wave = (std::sin(p * PI) * bounceScale);
      sprite->scale.x = 1.0f + wave * squashDirX;
      sprite->scale.y = 1.0f + wave * squashDirY;
      sprite->tint = Fade(RED, 0.8f);
    } else {
      auto sprite = gameObject->GetComponent<SpriteRenderer>();
      sprite->scale = {1.0f, 1.0f};
      sprite->tint = WHITE;
    }
  }

  void TakeDamage(float damage) {
    if (invincibilityTimer > 0.0f || hp <= 0.0f)
      return;

    hp -= damage;
    invincibilityTimer = invincibilityTime;
    bounceTimer.Reset(0.1f);

    if (MathUtils::GetRandom(0, 1) == 0) {
      squashDirX = 1.0f;
      squashDirY = -1.0f;
    } else {
      squashDirX = -1.0f;
      squashDirY = 1.0f;
    }

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
