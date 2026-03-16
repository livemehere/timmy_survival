#pragma once

#include <string>

struct EnemyConfig {
  std::string name;
  float maxHp;
  float damage;
  float speed;
  float attackRange;
  float attackCooldown;
  float bounceScale;
  float squashScale;
};

namespace Enemy {
inline const EnemyConfig KNIGHT = {
    .name = "Knight",
    .maxHp = 3.0f,
    .damage = 1.0f,
    .speed = 35.0f,
    .attackRange = 20.0f,
    .attackCooldown = 1.0f,
    .bounceScale = 0.15f,
    .squashScale = 0.15f,

};
}
