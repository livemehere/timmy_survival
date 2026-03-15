#include "Health.hpp"
#include "../Core/GameObject.hpp"
#include "../Utils/MathUtils.hpp"
#include "Render/SpriteRenderer.hpp"
#include <cmath>

void Health::Update(float dt) {
  invincibilityTimer.Update(dt);
  hitTimer.Update(dt);

  auto sprite = gameObject->GetComponent<SpriteRenderer>();
  if (!sprite) {
    return;
  }

  if (hitTimer.IsRunning()) {
    float p = 1.0f - hitTimer.GetProgress();
    float wave = std::sin(p * PI) * bounceScale;
    sprite->scale.x = 1.0f + wave * squashDirX;
    sprite->scale.y = 1.0f + wave * squashDirY;
    sprite->tint = Fade(RED, 0.8f);
    return;
  }

  sprite->scale = {1.0f, 1.0f};
  sprite->tint = WHITE;
}

void Health::TakeDamage(float damage) {
  if (invincibilityTimer.IsRunning() || hp <= 0.0f) {
    return;
  }

  hp -= damage;
  invincibilityTimer.Reset(invincibilityTime);
  hitTimer.Reset(0.1f);

  if (onDamage) {
    onDamage(damage);
  }

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

void Health::Die() {
  if (onDeath) {
    onDeath();
  }

  // TODO: in future, not directly remove, but play death animation first
  gameObject->Destroy();
}

void Health::Draw() {
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
