#pragma once

#include "object.hpp"

#include <unordered_map>

namespace figine::core {

class scene_t {
  std::unordered_map<std::string, object_if *> objects;

  inline virtual void init() const {
    for (const auto &[key, object] : objects) {
      object->init();
    }
  }

  inline virtual void update() const {
    for (const auto &[key, object] : objects) {
      object->update();
    }
  }
};

} // namespace figine::core
