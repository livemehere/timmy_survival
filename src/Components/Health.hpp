#pragma once

#include "../Core/Component.hpp"
#include "../Core/Timer.hpp"
#include <functional>

class Health : public Component {
public:
  float maxHp;
  float hp;

  float invincibilityTime = 0.1f;
  Timer invincibilityTimer = Timer(0.0f, false);

  std::function<void()> onDeath;
  std::function<void(float damage)> onDamage;

  Timer hitTimer = Timer(0.0f, false);
  float bounceScale = 0.15f;
  float squashDirX = 1.0f;
  float squashDirY = -1.0f;

  Health(float maxHp) : maxHp(maxHp), hp(maxHp) {}

  void Update(float dt) override;

  void TakeDamage(float damage);

  void Die();

  void Draw() override;
};
