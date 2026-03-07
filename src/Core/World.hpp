#pragma once

#include "Collider.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/CircleCollider.hpp"
#include "GameObject.hpp"
#include <memory>
#include <string>
#include <vector>

class World {
public:
  std::vector<std::shared_ptr<GameObject>> objects;

  GameObject *CreateObject(std::string name);
  void Update(float dt);
  void Draw();
  std::vector<Collider *> GetAllColliders();

  GameObject *GetObjectByName(std::string name);
  void ResolveCollisions();
  void ResolveCircleCircleCollision(CircleCollider *a, CircleCollider *b);
  void ResolveBoxBoxCollision(BoxCollider *a, BoxCollider *b);
  void ResolveCircleBoxCollision(CircleCollider *circle, BoxCollider *box);
};
