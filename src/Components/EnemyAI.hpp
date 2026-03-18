#pragma once

#include "../Core/Component.hpp"
#include "../Core/Timer.hpp"
#include "raylib.h"

enum class EnemyState {
  CHASE,     // Basic pursuit
  CHARGING,  // Charging (attack wind-up)
  ATTACK,    // Attack
  COOLDOWN   // Post-attack cooldown
};

class EnemyAI : public Component {
public:
  GameObject *target = nullptr;
  float speed;
  float damage;
  float attackRange;
  float chargeDuration;
  float dashDuration;
  float dashSpeedMultiplier;
  float attackCooldown;

  EnemyAI(GameObject *target, float speed, float damage, float attackRange,
          float chargeDuration, float dashDuration, float dashSpeedMultiplier,
          float attackCooldown)
      : target(target), speed(speed), damage(damage), attackRange(attackRange),
        chargeDuration(chargeDuration), dashDuration(dashDuration),
        dashSpeedMultiplier(dashSpeedMultiplier), attackCooldown(attackCooldown) {}

  void Update(float dt) override;
  void Draw() override;

private:
  EnemyState state = EnemyState::CHASE;
  Timer stateTimer = Timer(0.0f, false);
  Timer cooldownTimer = Timer(0.0f, false);

  Vector2 dashDirection;
};
