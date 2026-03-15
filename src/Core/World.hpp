#pragma once

#include "../Components/BoxCollider.hpp"
#include "../Components/CircleCollider.hpp"
#include "Collider.hpp"
#include "GameObject.hpp"
#include <memory>
#include <string>
#include <vector>

class World {
public:
  std::vector<std::shared_ptr<GameObject>> objects;
  std::vector<Collider *> activeColliders;

  World() = default;
  ~World();

  GameObject *CreateObject(std::string name);
  void Update(float dt);
  void DrawBg();
  void Draw();
  void DrawUI();

  GameObject *GetObjectByName(std::string name);
  std::vector<GameObject *> GetObjectsByLayer(Layer layer);

  void RegisterCollider(Collider *col);
  void UnregisterCollider(Collider *col);
  void ResolveCollisions();
  void ResolveCircleCircleCollision(CircleCollider *a, CircleCollider *b);
  void ResolveBoxBoxCollision(BoxCollider *a, BoxCollider *b);
  void ResolveCircleBoxCollision(CircleCollider *circle, BoxCollider *box);
  void HandleTriggerEvent(Collider *a, Collider *b);
};
