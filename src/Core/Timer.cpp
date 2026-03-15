#include "Timer.hpp"
#include <algorithm>
#include <cmath>

Timer::Timer(float target, bool loop)
    : targetTime(std::max(0.0f, target)), isLooping(loop),
      isRunning(targetTime > 0.0f) {}

void Timer::Update(float dt) {
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

void Timer::Reset() { Start(); }

void Timer::Reset(float newTarget) {
  SetTargetTime(newTarget);
  Start();
}

void Timer::Start() {
  currentTime = 0.0f;
  isRunning = targetTime > 0.0f;
  completedThisFrame = false;
}

void Timer::Stop() {
  currentTime = 0.0f;
  isRunning = false;
  completedThisFrame = false;
}

void Timer::SetTargetTime(float newTarget) {
  targetTime = std::max(0.0f, newTarget);
  if (targetTime <= 0.0f) {
    Stop();
    return;
  }

  if (currentTime > targetTime) {
    currentTime = targetTime;
  }
}

void Timer::SetLooping(bool loop) { isLooping = loop; }

float Timer::GetCurrentTime() const { return currentTime; }

float Timer::GetTargetTime() const { return targetTime; }

float Timer::GetProgress() const {
  if (targetTime <= 0.0f) {
    return 1.0f;
  }

  return std::clamp(currentTime / targetTime, 0.0f, 1.0f);
}

bool Timer::DidCompleteThisFrame() const { return completedThisFrame; }

bool Timer::IsLooping() const { return isLooping; }

bool Timer::IsRunning() const { return isRunning; }
