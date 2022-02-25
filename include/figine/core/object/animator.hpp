#pragma once

#include "animation.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace figine::core {

class animator_t {
public:
  animator_t(animation_t *animation);

  std::vector<glm::mat4> bone_matrices;

  void update(float dt);
  void set_animation(animation_t *animation);
  void exec(const std::unordered_map<std::string, glm::mat4> target_map);

private:
  void calc(const bone_data_t *node, glm::mat4 parent_transform);
  void calc_exec(const bone_data_t *node, glm::mat4 parent_transform,
                 const std::unordered_map<std::string, glm::mat4> &target_map);

  animation_t *current_animation;
  float current_time;
  float delta_time;
};

} // namespace figine::core