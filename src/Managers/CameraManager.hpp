#pragma once

#include "../Core/Timer.hpp"
#include "raylib.h"

class CameraManager {
private:
  Camera2D camera;
  Vector2 *targetPos;
  Vector2 offsetRatio;
  float zoom;
  float rotation;
  float lerpSpeed;

  // target drawing
  bool showReticle;
  float reticleSize;
  float reticleThickness;
  Color reticleColor;

  Timer shakeTimer;
  float shakeIntensity;

  CameraManager() = default;
  ~CameraManager() = default;

public:
  static CameraManager &Get();

  CameraManager(const CameraManager &) = delete;
  CameraManager &operator=(const CameraManager &) = delete;

  void Init();
  void Clear();
  void Update(float dt);
  void SetTarget(Vector2 *newTarget);
  void SetOffsetRatio(Vector2 newOffsetRatio);
  Camera2D GetCamera();
  void Draw();
  void Shake(float intensity, float duration);
};
