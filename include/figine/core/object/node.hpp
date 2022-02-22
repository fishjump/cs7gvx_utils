#pragma once

#include "figine/dep.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace figine::core {

struct assimp_node_data_t {
  glm::mat4 transform;
  std::string name;
  std::vector<assimp_node_data_t> children;
};

} // namespace figine::core
