#pragma once

#include <functional>
#include <unordered_map>

template <typename... Args> class Event {
private:
  int seq = 0;
  std::unordered_map<int, std::function<void(Args...)>> listeners;

public:
  int AddListener(std::function<void(Args...)> listener) {
    int id = seq++;
    listeners[id] = listener;
    return id;
  }

  void Invoke(Args... args) {
    for (auto &[id, listener] : listeners) {
      listener(args...);
    }
  }

  void RemoveListener(int id) { listeners.erase(id); }

  void Clear() { listeners.clear(); }

  bool HasListeners() const { return !listeners.empty(); }
};
