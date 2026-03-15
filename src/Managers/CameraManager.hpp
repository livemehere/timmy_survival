#pragma once

#include "../Core/Timer.hpp"
#include "../Utils/MathUtils.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

constexpr float minZoom = 0.1f;
constexpr float maxZoom = 3.0f;

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
  static CameraManager &Get() {
    static CameraManager instance;
    return instance;
  }

  CameraManager(const CameraManager &) = delete;
  CameraManager &operator=(const CameraManager &) = delete;

  void Init() {
    targetPos = nullptr;
    offsetRatio = {0.5f, 0.5f};
    zoom = 3.0f;
    rotation = 0.0f;
    lerpSpeed = 5.0f;

    showReticle = true;
    reticleSize = 20.0f;
    reticleThickness = 1.2f;
    reticleColor = LIME;

    shakeIntensity = 0.0f;

    camera.target = {0, 0};
    camera.offset = {GetScreenWidth() * offsetRatio.x,
                     GetScreenHeight() * offsetRatio.y};
    camera.rotation = rotation;
    camera.zoom = zoom;
  }

  void Clear() {
    targetPos = nullptr;
    shakeIntensity = 0.0f;
    shakeTimer.Reset(0.0f);
  }

  void Update(float dt) {

    Vector2 defaultOffset = {GetScreenWidth() * offsetRatio.x,
                             GetScreenHeight() * offsetRatio.y};
    if (targetPos) {
      camera.target = Vector2Lerp(camera.target, *targetPos, lerpSpeed * dt);
      camera.offset = Vector2Lerp(camera.offset, defaultOffset, lerpSpeed * dt);
      camera.zoom = Lerp(camera.zoom, zoom, lerpSpeed * dt);
      camera.rotation = Lerp(camera.rotation, rotation, lerpSpeed * dt);
    }

    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
      zoom += wheelMove * 0.1f;
      zoom = std::min(std::max(minZoom, zoom), maxZoom);
    }

    shakeTimer.Update(dt);

    if (shakeTimer.IsRunning()) {
      float shakeOffsetX =
          MathUtils::GetRandom(-shakeIntensity, shakeIntensity);
      float shakeOffsetY =
          MathUtils::GetRandom(-shakeIntensity, shakeIntensity);
      camera.offset = Vector2Add(defaultOffset, {shakeOffsetX, shakeOffsetY});
    } else {
      shakeIntensity = 0.0f;
      camera.offset = defaultOffset;
    }
  }

  void SetTarget(Vector2 *newTarget) { targetPos = newTarget; }
  void SetOffsetRatio(Vector2 newOffsetRatio) { offsetRatio = newOffsetRatio; }

  Camera2D GetCamera() { return camera; }
  void Draw() {
    if (!showReticle)
      return;

    Vector2 pos = camera.target;

    float s = reticleSize / camera.zoom;
    float t = reticleThickness / camera.zoom;
    float gap = s * 0.3f;

    // lt
    DrawLineEx({pos.x - s, pos.y - s}, {pos.x - gap, pos.y - s}, t,
               reticleColor);
    DrawLineEx({pos.x - s, pos.y - s}, {pos.x - s, pos.y - gap}, t,
               reticleColor);

    // rt
    DrawLineEx({pos.x + s, pos.y - s}, {pos.x + gap, pos.y - s}, t,
               reticleColor);
    DrawLineEx({pos.x + s, pos.y - s}, {pos.x + s, pos.y - gap}, t,
               reticleColor);

    // lb
    DrawLineEx({pos.x - s, pos.y + s}, {pos.x - gap, pos.y + s}, t,
               reticleColor);
    DrawLineEx({pos.x - s, pos.y + s}, {pos.x - s, pos.y + gap}, t,
               reticleColor);

    // rb
    DrawLineEx({pos.x + s, pos.y + s}, {pos.x + gap, pos.y + s}, t,
               reticleColor);
    DrawLineEx({pos.x + s, pos.y + s}, {pos.x + s, pos.y + gap}, t,
               reticleColor);

    // center
    DrawCircleV(pos, t * 1.5f, reticleColor);
  }

  void Shake(float intensity, float duration) {
    shakeIntensity = intensity;
    shakeTimer.Reset(duration);
  }
};
