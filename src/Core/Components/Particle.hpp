#pragma once

#include "../../Utils/MathUtils.hpp"
#include "../GameObject.hpp"
#include "raylib.h"
#include "raymath.h"

class Particle : public Component {
public:
  Vector2 velocity;
  float lifetime;
  float maxLifetime;
  Color color;
  float size;
  float friction;
  float rotation;

  Particle(Vector2 velocity, float lifetime, Color color, float size,
           float friction)
      : velocity(velocity), lifetime(lifetime), maxLifetime(lifetime),
        color(color), size(size), friction(friction) {
    rotation = MathUtils::GetRandom(0.0f, 360.0f);
  }

  void Update(float dt) override {

    lifetime -= dt;
    if (lifetime <= 0.0f) {
      gameObject->Destroy();
      return;
    }

    gameObject->position =
        Vector2Add(gameObject->position, Vector2Scale(velocity, friction * dt));
    velocity = Vector2Lerp(velocity, {0, 0}, friction * dt);
    rotation += (Vector2Length(velocity) * dt);
  }

  void Draw() override {
    float ratio = lifetime / maxLifetime;
    float curScale = size * ratio;
    Color curColor = Fade(color, ratio);

    Rectangle rect = {gameObject->position.x, gameObject->position.y, curScale,
                      curScale};

    DrawRectanglePro(rect, {curScale / 2, curScale / 2}, rotation, curColor);
  }
};
