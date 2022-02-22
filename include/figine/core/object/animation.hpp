#pragma once

#include "bone.hpp"
#include "node.hpp"
#include "object.hpp"

#include <unordered_map>

namespace figine::core {

class animation_t {
public:
  float duration;
  int ticks_per_second;
  std::vector<bone_t> bones;
  assimp_node_data_t root;
  std::unordered_map<std::string, bone_info_t> bone_info_map;

  animation_t() = default;
  animation_t(const std::string &file, object_t &obj);
  bone_t *find_bone(const std::string &name);

private:
  void read_missing_bones(const aiAnimation *animation, object_t &obj);
  void read_heirarchy_data(assimp_node_data_t &dest, const aiNode *src);
};

} // namespace figine::core
