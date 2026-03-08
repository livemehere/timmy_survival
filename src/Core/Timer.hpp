#pragma once

class Timer {
public:
  float targetTime;
  float currentTime;
  bool isLooping;
  Timer(float target, bool loop = true) {
    targetTime = target;
    currentTime = 0.0f;
    isLooping = loop;
  }

  bool Update(float dt) {
    currentTime += dt;

    if (currentTime >= targetTime) {
      if (isLooping) {
        currentTime -= targetTime;
      }
      return true;
    }

    return false;
  }

  void Reset() { currentTime = 0.0f; }

  void SetTargetTime(float newTarget) { targetTime = newTarget; }
};
