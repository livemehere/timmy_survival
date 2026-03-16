#include "OrbitWeapon.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../Colliders/CircleCollider.hpp"
#include "../Render/SpriteRenderer.hpp"
#include "OrbitBlade.hpp"
#include "raymath.h"
#include <algorithm>
#include <cmath>
#include <string>

namespace {
SpriteRenderer *ApplySpritePreset(GameObject *gameObject,
                                  const SpritePreset &preset) {
  auto sprite = gameObject->AddComponent<SpriteRenderer>();

  for (const auto &clip : preset.clips) {
    sprite->AddClip(clip);
  }

  sprite->scale = preset.scale;
  sprite->anchorRatio = preset.anchorRatio;
  sprite->offset = preset.offset;
  sprite->tint = preset.tint;

  if (!preset.defaultClipName.empty()) {
    sprite->Play(preset.defaultClipName);
  }

  return sprite;
}
} // namespace

void OrbitWeapon::Start() { SyncOrbitObjects(); }

void OrbitWeapon::Update(float dt) {
  if (!owner || !owner->isAlive) {
    gameObject->Destroy();
    return;
  }

  SyncOrbitObjects();
  orbitRotationDegrees += orbitSpeedDegrees * dt;
  UpdateOrbitTransforms();
}

void OrbitWeapon::OnDestroy() {
  for (auto *orbitObject : orbitObjects) {
    if (orbitObject && orbitObject->isAlive) {
      orbitObject->Destroy();
    }
  }

  orbitObjects.clear();
}

void OrbitWeapon::SyncOrbitObjects() {
  orbitObjects.erase(
      std::remove_if(orbitObjects.begin(), orbitObjects.end(),
                     [](GameObject *obj) { return !obj || !obj->isAlive; }),
      orbitObjects.end());

  count = std::max(0, count);

  while (static_cast<int>(orbitObjects.size()) > count) {
    auto *orbitObject = orbitObjects.back();
    if (orbitObject && orbitObject->isAlive) {
      orbitObject->Destroy();
    }
    orbitObjects.pop_back();
  }

  while (static_cast<int>(orbitObjects.size()) < count) {
    int index = static_cast<int>(orbitObjects.size());
    auto *orbitObject =
        gameObject->world->CreateObject(gameObject->name + "_orbit_" +
                                        std::to_string(index));
    orbitObject->layer = Layer::WEAPON;
    orbitObject->position = owner ? owner->position : gameObject->position;

    auto collider = orbitObject->AddComponent<CircleCollider>(colliderRadius);
    collider->isTrigger = true;

    orbitObject->AddComponent<OrbitBlade>(damage, knockbackForce, hitCooldown);
    ApplySpritePreset(orbitObject, orbitSpritePreset);
    orbitObjects.push_back(orbitObject);
  }
}

void OrbitWeapon::UpdateOrbitTransforms() {
  if (!owner || orbitObjects.empty()) {
    return;
  }

  float stepDegrees = 360.0f / static_cast<float>(orbitObjects.size());
  for (size_t i = 0; i < orbitObjects.size(); i++) {
    auto *orbitObject = orbitObjects[i];
    if (!orbitObject || !orbitObject->isAlive) {
      continue;
    }

    float angleDegrees = orbitRotationDegrees + (stepDegrees * i);
    float angleRadians = angleDegrees * DEG2RAD;
    Vector2 orbitOffset = {std::cos(angleRadians) * orbitRadius,
                           std::sin(angleRadians) * orbitRadius};
    orbitObject->position = Vector2Add(owner->position, orbitOffset);

    auto sprite = orbitObject->GetComponent<SpriteRenderer>();
    if (sprite) {
      sprite->rotation = angleDegrees + 90.0f;
    }
  }
}
