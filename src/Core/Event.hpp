#pragma once

#include <functional>
#include <vector>

template <typename... Args> class Event {
private:
  std::vector<std::function<void(Args...)>> listeners;

public:
  void AddListener(std::function<void(Args...)> listener) {
    listeners.push_back(listener);
  }

  void Invoke(Args... args) {
    for (auto &listener : listeners) {
      listener(args...);
    }
  }

  void Clear() { listeners.clear(); }

  bool HasListeners() const { return !listeners.empty(); }
};
