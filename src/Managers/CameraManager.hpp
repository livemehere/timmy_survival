#pragma once

#include "../Core/Timer.hpp"
#include "../Utils/MathUtils.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <iostream>

constexpr float minZoom = 0.1f;
constexpr float maxZoom = 3.0f;

class CameraManager {
private:
  Camera2D camera;
  Vector2 *targetPos = nullptr;
  Vector2 offsetRatio = {0.5f, 0.5f};
  float zoom = 3.0f;
  float rotation = 0.0f;
  float lerpSpeed = 5.0f;

  // target drawing
  bool showReticle = true;
  float reticleSize = 20.0f;
  float reticleThickness = 1.2f;
  Color reticleColor = LIME;

  Timer shakeTimer = Timer(0.0f, false);
  float shakeIntensity = 0.0f;

public:
  CameraManager() {
    camera.target = {0, 0};
    camera.offset = offsetRatio;
    camera.rotation = rotation;
    camera.zoom = zoom;
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

    if (!shakeTimer.Update(dt)) {
      // if shaking...
      Vector2 baseOffset = {GetScreenWidth() * offsetRatio.x,
                            GetScreenHeight() * offsetRatio.y};
      float shakeOffsetX =
          MathUtils::GetRandom(-shakeIntensity, shakeIntensity);
      float shakeOffsetY =
          MathUtils::GetRandom(-shakeIntensity, shakeIntensity);
      camera.offset = Vector2Add(baseOffset, {shakeOffsetX, shakeOffsetY});
    } else {
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
