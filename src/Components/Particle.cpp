#include "Particle.hpp"

#include "../Core/GameObject.hpp"
#include "raymath.h"

void Particle::Update(float dt) {
  lifeTimer.Update(dt);
  if (!lifeTimer.IsRunning()) {
    gameObject->Destroy();
    return;
  }

  gameObject->position =
      Vector2Add(gameObject->position, Vector2Scale(velocity, friction * dt));
  velocity = Vector2Lerp(velocity, {0.0f, 0.0f}, friction * dt);
  rotation += Vector2Length(velocity) * dt;
}

void Particle::Draw() {
  float ratio = 1.0f - lifeTimer.GetProgress();
  float curScale = size * ratio;
  Color curColor = Fade(color, ratio);

  Rectangle rect = {gameObject->position.x, gameObject->position.y, curScale,
                    curScale};

  DrawRectanglePro(rect, {curScale / 2, curScale / 2}, rotation, curColor);
}
