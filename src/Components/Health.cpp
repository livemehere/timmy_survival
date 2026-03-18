#include "Health.hpp"
#include "../Core/GameObject.hpp"
#include "../Utils/MathUtils.hpp"
#include "Render/SpriteRenderer.hpp"
#include <cmath>

void Health::Update(float dt) {
  invincibilityTimer.Update(dt);
  hitTimer.Update(dt);

  for (auto it = sourceHitTimers.begin(); it != sourceHitTimers.end();) {
    it->second.Update(dt);

    if (!it->second.IsRunning()) {
      it = sourceHitTimers.erase(it);
      continue;
    }

    ++it;
  }

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

bool Health::TakeDamage(float damage, GameObject *source,
                        float sourceInvincibilityTime) {
  if (hp <= 0.0f) {
    return false;
  }

  if (useSharedInvincibility && invincibilityTimer.IsRunning()) {
    return false;
  }

  if (source) {
    auto it = sourceHitTimers.find(source);
    if (it != sourceHitTimers.end() && it->second.IsRunning()) {
      return false;
    }
  } else if (invincibilityTimer.IsRunning()) {
    return false;
  }

  hp -= damage;

  if (source) {
    float cooldown = sourceInvincibilityTime >= 0.0f ? sourceInvincibilityTime
                                                     : invincibilityTime;
    sourceHitTimers[source].Reset(cooldown);
    if (useSharedInvincibility) {
      invincibilityTimer.Reset(cooldown);
    }
  } else {
    invincibilityTimer.Reset(invincibilityTime);
  }

  hitTimer.Reset(0.1f);

  if (onDamage) {
    onDamage(damage, source);
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

  return true;
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

  float barW = isBoss ? 220.0f : 20.0f;
  float barH = isBoss ? 10.0f : 3.0f;
  float yOffset = isBoss ? 34.0f : 15.0f;
  float hpRatio = hp / maxHp;

  Rectangle bgRec = {pos.x - barW / 2.0f, pos.y - yOffset, barW, barH};
  Rectangle fgRec = {pos.x - barW / 2.0f, pos.y - yOffset, barW * hpRatio,
                     barH};

  // Background color (very dark gray)
  Color bgColor = isBoss ? (Color){30, 30, 30, 255} : DARKGRAY;
  DrawRectangleRec(bgRec, bgColor);

  // HP bar color (boss: ORANGE(60%+) -> RED(30-60%) -> MAROON(<30%))
  Color healthColor;
  if (!isBoss) {
    healthColor = MAROON;
  } else {
    if (hpRatio > 0.6f) {
      healthColor = ORANGE;
    } else if (hpRatio > 0.3f) {
      healthColor = RED;
    } else {
      healthColor = MAROON;
    }
  }
  DrawRectangleRec(fgRec, healthColor);

  // Border (boss: gold, normal: black)
  Color borderColor = isBoss ? GOLD : BLACK;
  float borderThickness = isBoss ? 1.5f : 0.5f;
  DrawRectangleLinesEx(bgRec, borderThickness, borderColor);

  // Extra boss health bar effect
  if (isBoss) {
    // Inner gold border
    DrawRectangleLinesEx(bgRec, 0.8f, Fade(GOLD, 0.7f));
  }
}
