#pragma once

#include "raylib.h"
#include "raymath.h"
#include <iostream>

class CameraManager {
private:
  Camera2D camera;
  Vector2 *targetPos = nullptr;
  Vector2 offsetRatio = {0.5f, 0.5f};
  float zoom = 1.0f;
  float rotation = 0.0f;
  float lerpSpeed = 5.0f;

public:
  CameraManager() {
    camera.target = {0, 0};
    camera.offset = offsetRatio;
    camera.rotation = rotation;
    camera.zoom = zoom;
  }

  void Update(float dt) {

    if (targetPos) {

      Vector2 offset = {GetScreenWidth() * offsetRatio.x,
                        GetScreenHeight() * offsetRatio.y};
      camera.target = Vector2Lerp(camera.target, *targetPos, lerpSpeed * dt);
      camera.offset = Vector2Lerp(camera.offset, offset, lerpSpeed * dt);
      camera.zoom = Lerp(camera.zoom, zoom, lerpSpeed * dt);
      camera.rotation = Lerp(camera.rotation, rotation, lerpSpeed * dt);
    }
  }

  void SetTarget(Vector2 *newTarget) { targetPos = newTarget; }
  void SetOffsetRatio(Vector2 newOffsetRatio) { offsetRatio = newOffsetRatio; }

  Camera2D GetCamera() { return camera; }
};
