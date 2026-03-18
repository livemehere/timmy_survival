#include "EnemyAI.hpp"
#include "../Components/Health.hpp"
#include "../Core/GameObject.hpp"
#include "Render/SpriteRenderer.hpp"
#include "raylib.h"
#include "raymath.h"

void EnemyAI::Update(float dt) {
  if (!target || !target->isAlive) {
    return;
  }

  Vector2 myPos = gameObject->position;
  Vector2 targetPos = target->position;
  Vector2 dir = Vector2Subtract(targetPos, myPos);
  float distToTarget = Vector2Length(dir);

  if (Vector2LengthSqr(dir) > 0.0f) {
    dir = Vector2Normalize(dir);
  }

  auto sprite = gameObject->GetComponent<SpriteRenderer>();
  if (sprite) {
    sprite->flipX = dir.x < 0;

    // Apply a green tint while charging (gets stronger over time)
    if (state == EnemyState::CHARGING) {
      float progress = 1.0f - stateTimer.GetProgress(); // 0~1
      float greenIntensity = 0.3f + (progress * 0.7f); // 0.3 ~ 1.0
      sprite->tint = (Color){0, (unsigned char)(greenIntensity * 255), 0, 255};
    } else {
      sprite->tint = WHITE;
    }
  }

  // State machine
  switch (state) {
  case EnemyState::CHASE: {
    // Chase state: start charging when entering attack range
    gameObject->position.x += dir.x * speed * dt;
    gameObject->position.y += dir.y * speed * dt;

    if (distToTarget <= attackRange) {
      state = EnemyState::CHARGING;
      stateTimer.Reset(chargeDuration);
    }
    break;
  }

  case EnemyState::CHARGING: {
    // Charging state: charge in place
    stateTimer.Update(dt);
    if (!stateTimer.IsRunning()) {
      state = EnemyState::ATTACK;
      stateTimer.Reset(0.1f); // Attack frame lasts 0.1 seconds
    }
    break;
  }

  case EnemyState::ATTACK: {
    // Attack state: perform the melee attack
    stateTimer.Update(dt);

    // Deal damage if the target is within attack range
    if (distToTarget < attackRange + 10.0f) {
      auto playerHealth = target->GetComponent<Health>();
      if (playerHealth && playerHealth->TakeDamage(damage, gameObject, 0.8f)) {
        // Damage applied successfully (0.8 seconds of invincibility)
      }
    }

    if (!stateTimer.IsRunning()) {
      state = EnemyState::COOLDOWN;
      cooldownTimer.Reset(attackCooldown);
    }
    break;
  }

  case EnemyState::COOLDOWN: {
    // Cooldown state: slowly continue pursuing
    cooldownTimer.Update(dt);
    float cooldownSpeed = speed * 0.3f; // Move at 30% speed
    gameObject->position.x += dir.x * cooldownSpeed * dt;
    gameObject->position.y += dir.y * cooldownSpeed * dt;

    if (!cooldownTimer.IsRunning()) {
      state = EnemyState::CHASE;
    }
    break;
  }
  }
}

void EnemyAI::Draw() {
  // Charging bar removed; tint is used instead
}
