#pragma once

#include "figine/dep.hpp"

#include <assimp/scene.h>

#include <vector>

namespace figine::core {

struct key_position_t {
  glm::vec3 position;
  float timestamp;
};

struct key_rotation_t {
  glm::quat orientation;
  float timestamp;
};

struct key_scale_t {
  glm::vec3 scale;
  float timestamp;
};

struct bone_info_t {
  int id;
  glm::mat4 offset;
};

class bone_t {
public:
  bone_t(const std::string &name, int id, const aiNodeAnim *channel);

  const int id;
  const std::string name;

  glm::mat4 local_transform;

  void update(float animation_time);

private:
  float get_scale_factor(float last_ts, float next_ts,
                       float animation_time);

  int get_position_index(float animation_time);
  int get_rotation_index(float animation_time);
  int get_scale_index(float animation_time);

  glm::mat4 interpolate_position(float animation_time);
  glm::mat4 interpolate_rotation(float animation_time);
  glm::mat4 interpolate_scaling(float animation_time);

  std::vector<key_position_t> positions;
  std::vector<key_rotation_t> rotations;
  std::vector<key_scale_t> scales;
};

} // namespace figine::core