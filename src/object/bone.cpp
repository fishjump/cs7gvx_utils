#include "figine/core/object/bone.hpp"
#include "assimp_to_glm_cvt.hpp"
#include "figine/logging.hpp"

#include <glm/gtx/string_cast.hpp>

namespace figine::core {

bone_t::bone_t(const std::string &name, int id, const aiNodeAnim *channel)
    : id(id), name(name), local_transform(1.0f) {
  for (int i = 0; i < channel->mNumPositionKeys; i++) {
    key_position_t data = {
        .position = _object_internal::assimp_vec3_to_glm_vec3(
            channel->mPositionKeys[i].mValue),
        .timestamp = channel->mPositionKeys[i].mTime,
    };

    positions.push_back(data);
  }

  for (int i = 0; i < channel->mNumRotationKeys; i++) {
    key_rotation_t data = {
        .orientation = _object_internal::assimp_quat_to_glm_quat(
            channel->mRotationKeys[i].mValue),
        .timestamp = channel->mRotationKeys[i].mTime,
    };

    rotations.push_back(data);
  }

  for (int i = 0; i < channel->mNumScalingKeys; i++) {
    key_scale_t data = {
        .scale = _object_internal::assimp_vec3_to_glm_vec3(
            channel->mScalingKeys[i].mValue),
        .timestamp = channel->mScalingKeys[i].mTime,
    };

    scales.push_back(data);
  }
}

void bone_t::update(float animation_time) {
  glm::mat4 translation = interpolate_position(animation_time);
  glm::mat4 rotation = interpolate_rotation(animation_time);
  glm::mat4 scale = interpolate_scaling(animation_time);
  local_transform = translation * rotation * scale;
}

int bone_t::get_position_index(float animation_time) {
  for (int i = 0; i < positions.size() - 1; i++) {
    if (animation_time < positions[i + 1].timestamp) {
      return i;
    }
  }

  return positions.size() - 1;
}

int bone_t::get_rotation_index(float animation_time) {
  for (int i = 0; i < rotations.size() - 1; i++) {
    if (animation_time < rotations[i + 1].timestamp) {
      return i;
    }
  }

  return rotations.size() - 1;
}

int bone_t::get_scale_index(float animation_time) {
  for (int i = 0; i < scales.size() - 1; i++) {
    if (animation_time < scales[i + 1].timestamp) {
      return i;
    }
  }

  return scales.size() - 1;
}

float bone_t::get_scale_factor(float last_ts, float next_ts,
                               float animation_time) {
  float mid_way_len = animation_time - last_ts;
  float frame_diff = next_ts - last_ts;

  return mid_way_len / frame_diff;
}

glm::mat4 bone_t::interpolate_position(float animation_time) {
  if (positions.size() == 1) {
    return glm::translate(glm::mat4(1.0f), positions[0].position);
  }

  int prev_idx = get_position_index(animation_time);
  int next_idx = prev_idx + 1;
  float scale_factor =
      get_scale_factor(positions[prev_idx].timestamp,
                       positions[next_idx].timestamp, animation_time);

  glm::vec3 res = glm::mix(positions[prev_idx].position,
                           positions[next_idx].position, scale_factor);
  return glm::translate(glm::mat4(1.0f), res);
}

glm::mat4 bone_t::interpolate_rotation(float animation_time) {
  if (rotations.size() == 1) {
    auto rotation = glm::normalize(rotations[0].orientation);
    return glm::mat4(rotation);
  }

  int prev_idx = get_rotation_index(animation_time);
  int next_idx = prev_idx + 1;

  float scale_factor =
      get_scale_factor(rotations[prev_idx].timestamp,
                       rotations[next_idx].timestamp, animation_time);

  glm::quat res = glm::slerp(rotations[prev_idx].orientation,
                             rotations[next_idx].orientation, scale_factor);
  return glm::mat4(glm::normalize(res));
}

glm::mat4 bone_t::interpolate_scaling(float animation_time) {
  if (scales.size() == 1) {
    return glm::scale(glm::mat4(1.0f), scales[0].scale);
  }

  int prev_idx = get_scale_index(animation_time);
  int next_idx = prev_idx + 1;
  float scale_factor = get_scale_factor(
      scales[prev_idx].timestamp, scales[next_idx].timestamp, animation_time);

  glm::vec3 res =
      glm::mix(scales[prev_idx].scale, scales[next_idx].scale, scale_factor);
  return glm::scale(glm::mat4(1.0f), res);
}

} // namespace figine::core