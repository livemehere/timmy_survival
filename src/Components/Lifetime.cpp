#include "Lifetime.hpp"

#include "../Core/GameObject.hpp"

Lifetime::Lifetime(float duration) : timer(duration, false) {}

void Lifetime::Update(float dt) {
  timer.Update(dt);

  if (!timer.IsRunning()) {
    gameObject->Destroy();
  }
}
