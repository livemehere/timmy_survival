#include "Lifetime.hpp"

#include "../Core/GameObject.hpp"

void Lifetime::Update(float dt) {
  timer.Update(dt);

  if (!timer.IsRunning()) {
    gameObject->Destroy();
  }
}
