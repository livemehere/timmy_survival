#include "Collider.hpp"
#include "World.hpp"

Collider::Collider(ColliderType type, Vector2 offset)
    : type(type), offset(offset) {}

Collider::~Collider() {
  if (gameObject && gameObject->world) {
    gameObject->world->UnregisterCollider(this);
  }
  onTriggerEnter.Clear();
}

void Collider::Start() { gameObject->world->RegisterCollider(this); }
