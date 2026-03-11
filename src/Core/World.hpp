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
  CameraManager *cm = nullptr;
  GameManager *gameManager = nullptr;

  GameObject *CreateObject(std::string name);
  void Update(float dt);
  void DrawBg();
  void Draw();
  void DrawUI();
  std::vector<Collider *> GetAllColliders();

  GameObject *GetObjectByName(std::string name);
  void ResolveCollisions();
  void ResolveCircleCircleCollision(CircleCollider *a, CircleCollider *b);
  void ResolveBoxBoxCollision(BoxCollider *a, BoxCollider *b);
  void ResolveCircleBoxCollision(CircleCollider *circle, BoxCollider *box);
  void HandleTriggerEvent(Collider *a, Collider *b);
};
