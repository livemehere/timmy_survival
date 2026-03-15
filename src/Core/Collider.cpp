#include "Collider.hpp"
#include "World.hpp"

void Collider::OnDestroy() {
  if (gameObject && gameObject->world) {
    gameObject->world->UnregisterCollider(this);
  }
  onTriggerEnter.Clear();
}

void Collider::Start() { gameObject->world->RegisterCollider(this); }
