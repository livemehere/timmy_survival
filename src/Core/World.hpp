#pragma once

#include "../Managers/CameraManager.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/CircleCollider.hpp"
#include "Components/Collider.hpp"
#include "GameObject.hpp"
#include <memory>
#include <string>
#include <vector>

class GameManager;

class World {
public:
  std::vector<std::shared_ptr<GameObject>> objects;
  std::vector<Collider *> activeColliders;
  CameraManager *cm = nullptr;
  GameManager *gameManager = nullptr;

  GameObject *CreateObject(std::string name);
  void Update(float dt);
  void DrawBg();
  void Draw();
  void DrawUI();

  GameObject *GetObjectByName(std::string name);

  void RegisterCollider(Collider *col) { activeColliders.push_back(col); }
  void UnregisterCollider(Collider *col) {
    activeColliders.erase(
        std::remove(activeColliders.begin(), activeColliders.end(), col),
        activeColliders.end());
  }
  void ResolveCollisions();
  void ResolveCircleCircleCollision(CircleCollider *a, CircleCollider *b);
  void ResolveBoxBoxCollision(BoxCollider *a, BoxCollider *b);
  void ResolveCircleBoxCollision(CircleCollider *circle, BoxCollider *box);
  void HandleTriggerEvent(Collider *a, Collider *b);
};
