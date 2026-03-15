#include "CameraManager.hpp"
#include "../Utils/MathUtils.hpp"
#include "raymath.h"

#include <algorithm>

namespace {
constexpr float kMinZoom = 0.1f;
constexpr float kMaxZoom = 3.0f;
} // namespace

CameraManager::CameraManager() = default;

CameraManager::~CameraManager() = default;

CameraManager &CameraManager::Get() {
  static CameraManager instance;
  return instance;
}

void CameraManager::Init() {
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

void CameraManager::Clear() {
  targetPos = nullptr;
  shakeIntensity = 0.0f;
  shakeTimer.Reset(0.0f);
}

void CameraManager::Update(float dt) {
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
    zoom = std::min(std::max(kMinZoom, zoom), kMaxZoom);
  }

  shakeTimer.Update(dt);

  if (shakeTimer.IsRunning()) {
    float shakeOffsetX = MathUtils::GetRandom(-shakeIntensity, shakeIntensity);
    float shakeOffsetY = MathUtils::GetRandom(-shakeIntensity, shakeIntensity);
    camera.offset = Vector2Add(defaultOffset, {shakeOffsetX, shakeOffsetY});
  } else {
    shakeIntensity = 0.0f;
    camera.offset = defaultOffset;
  }
}

void CameraManager::SetTarget(Vector2 *newTarget) { targetPos = newTarget; }

void CameraManager::SetOffsetRatio(Vector2 newOffsetRatio) {
  offsetRatio = newOffsetRatio;
}

Camera2D CameraManager::GetCamera() { return camera; }

void CameraManager::Draw() {
  if (!showReticle) {
    return;
  }

  Vector2 pos = camera.target;

  float s = reticleSize / camera.zoom;
  float t = reticleThickness / camera.zoom;
  float gap = s * 0.3f;

  DrawLineEx({pos.x - s, pos.y - s}, {pos.x - gap, pos.y - s}, t, reticleColor);
  DrawLineEx({pos.x - s, pos.y - s}, {pos.x - s, pos.y - gap}, t, reticleColor);

  DrawLineEx({pos.x + s, pos.y - s}, {pos.x + gap, pos.y - s}, t, reticleColor);
  DrawLineEx({pos.x + s, pos.y - s}, {pos.x + s, pos.y - gap}, t, reticleColor);

  DrawLineEx({pos.x - s, pos.y + s}, {pos.x - gap, pos.y + s}, t, reticleColor);
  DrawLineEx({pos.x - s, pos.y + s}, {pos.x - s, pos.y + gap}, t, reticleColor);

  DrawLineEx({pos.x + s, pos.y + s}, {pos.x + gap, pos.y + s}, t, reticleColor);
  DrawLineEx({pos.x + s, pos.y + s}, {pos.x + s, pos.y + gap}, t, reticleColor);

  DrawCircleV(pos, t * 1.5f, reticleColor);
}

void CameraManager::Shake(float intensity, float duration) {
  shakeIntensity = intensity;
  shakeTimer.Reset(duration);
}
