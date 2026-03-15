#pragma once

#include "../../Core/Component.hpp"
#include "../../Core/Timer.hpp"
#include "raylib.h"
#include <deque>
#include <unordered_set>

class Projectile : public Component {
public:
  Timer lifeTimer;
  Timer tailTimer = Timer(0.05f, true);
  float damage = 1;
  int pierce;
  float knockbackForce;

  std::unordered_set<GameObject *> hitObjects;

  // effects
  std::deque<Vector2> history;
  Color color = LIME;
  int maxHistory = 10;
  float tailDelay = 0.05f;

  Projectile(float damage, float lifetime, int pierce = 1,
             float knockbackForce = 0.0f)
      : lifeTimer(lifetime, false), damage(damage), pierce(pierce),
        knockbackForce(knockbackForce) {}

  ~Projectile() = default;

  void Update(float dt) override;

  void OnTriggerEnter(Collider *other) override;

  void Draw() override;

  void SpawnParticles(Vector2 pos, int count);
};
