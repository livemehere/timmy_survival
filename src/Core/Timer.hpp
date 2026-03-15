#pragma once

#include <algorithm>

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

  void Update(float dt);

  void Reset();
  void Reset(float newTarget);
  void Start();
  void Stop();
  void SetTargetTime(float newTarget);
  void SetLooping(bool loop);

  float GetCurrentTime() const;
  float GetTargetTime() const;
  float GetProgress() const;

  bool DidCompleteThisFrame() const;
  bool IsLooping() const;
  bool IsRunning() const;
};
