#include "figine/core/object/animator.hpp"
#include "figine/logging.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace figine::core {

animator_t::animator_t(animation_t *animation) {
  current_time = 0.0;
  current_animation = animation;

  bone_matrices.reserve(100);

  for (int i = 0; i < 100; i++) {
    bone_matrices.push_back(glm::mat4(1.0f));
  }
}

void animator_t::update(float dt) {
  delta_time = dt;
  if (current_animation) {
    current_time += current_animation->ticks_per_second * dt;
    current_time = fmod(current_time, current_animation->duration);
    calc(&current_animation->root, glm::mat4(1.0f));
  }
}

void animator_t::set_animation(animation_t *animation) {
  current_animation = animation;
  current_time = 0.0f;
}

void animator_t::calc(const assimp_node_data_t *node,
                      glm::mat4 parent_transform) {
  std::string node_name = node->name;
  glm::mat4 node_transform = node->transform;

  bone_t *bone_t = current_animation->find_bone(node_name);

  if (bone_t) {
    bone_t->update(current_time);
    node_transform = bone_t->local_transform;
  }

  glm::mat4 global_transforma = parent_transform * node_transform;

  auto bone_info_map = current_animation->bone_info_map;
  if (bone_info_map.find(node_name) != bone_info_map.end()) {
    int index = bone_info_map[node_name].id;
    glm::mat4 offset = bone_info_map[node_name].offset;
    bone_matrices[index] = global_transforma * offset;
  }

  for (int i = 0; i < node->children.size(); i++) {
    calc(&node->children[i], global_transforma);
  }
}

} // namespace figine::core