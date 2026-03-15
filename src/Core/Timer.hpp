#pragma once

#include <algorithm>
#include <cmath>

class Timer {
private:
  float targetTime = 0.0f;
  float currentTime = 0.0f;
  bool isLooping = false;
  bool isRunning = false;
  bool completedThisFrame = false;

public:
  Timer() = default;

  explicit Timer(float target, bool loop = false)
      : targetTime(std::max(0.0f, target)), isLooping(loop),
        isRunning(targetTime > 0.0f) {}

  void Update(float dt) {
    completedThisFrame = false;

    if (!isRunning) {
      return;
    }

    currentTime += dt;

    if (currentTime < targetTime) {
      return;
    }

    // time's up
    completedThisFrame = true;

    if (isLooping) {
      currentTime = std::fmod(currentTime, targetTime);
    } else {
      currentTime = targetTime;
      isRunning = false;
    }
  }

  void Reset() { Start(); }

  void Reset(float newTarget) {
    SetTargetTime(newTarget);
    Start();
  }

  void Start() {
    currentTime = 0.0f;
    isRunning = targetTime > 0.0f;
    completedThisFrame = false;
  }

  void Stop() {
    currentTime = 0.0f;
    isRunning = false;
    completedThisFrame = false;
  }

  void SetTargetTime(float newTarget) {
    targetTime = std::max(0.0f, newTarget);
    if (targetTime <= 0.0f) {
      Stop();
      return;
    }

    if (currentTime > targetTime) {
      currentTime = targetTime;
    }
  }

  void SetLooping(bool loop) { isLooping = loop; }

  float GetCurrentTime() const { return currentTime; }
  float GetTargetTime() const { return targetTime; }
  float GetProgress() const {
    if (targetTime <= 0.0f) {
      return 1.0f;
    }

    return std::clamp(currentTime / targetTime, 0.0f, 1.0f);
  }

  bool DidCompleteThisFrame() const { return completedThisFrame; }
  bool IsLooping() const { return isLooping; }
  bool IsRunning() const { return isRunning; }
};
