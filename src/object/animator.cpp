#include "figine/core/object/animator.hpp"
#include "figine/logging.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/gtx/string_cast.hpp>
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

void animator_t::exec(
    const std::unordered_map<std::string, glm::mat4> target_map) {
  calc_exec(&current_animation->root, glm::mat4(1.0f), target_map);
}

void animator_t::calc_exec(
    const bone_data_t *node, glm::mat4 parent_transform,
    const std::unordered_map<std::string, glm::mat4> &target_map) {
  std::string node_name = node->name;
  glm::mat4 node_transform = node->transform;

  glm::mat4 global_transform = parent_transform * node_transform;
  if (auto iter = target_map.find(node_name); iter != target_map.end()) {
    global_transform = iter->second;
  }

  auto bone_info_map = current_animation->bone_info_map;
  if (bone_info_map.find(node_name) != bone_info_map.end()) {
    int index = bone_info_map[node_name].id;
    bone_matrices[index] = global_transform * bone_info_map[node_name].offset;
  }

  for (int i = 0; i < node->children.size(); i++) {
    calc_exec(&node->children[i], global_transform, target_map);
  }
}

void animator_t::calc(const bone_data_t *node, glm::mat4 parent_transform) {
  std::string node_name = node->name;
  glm::mat4 node_transform = node->transform;

  bone_t *bone_t = current_animation->find_bone(node_name);

  if (bone_t) {
    bone_t->update(current_time);
    node_transform = bone_t->local_transform;
  }

  glm::mat4 global_transform = parent_transform * node_transform;

  auto bone_info_map = current_animation->bone_info_map;
  if (bone_info_map.find(node_name) != bone_info_map.end()) {
    int index = bone_info_map[node_name].id;
    glm::mat4 offset = bone_info_map[node_name].offset;
    bone_matrices[index] = global_transform * offset;
  }

  for (int i = 0; i < node->children.size(); i++) {
    calc(&node->children[i], global_transform);
  }
}

} // namespace figine::core