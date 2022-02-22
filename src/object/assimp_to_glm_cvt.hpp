#pragma once

#include "figine/dep.hpp"

#include <assimp/scene.h>

namespace figine::core::_object_internal {

inline glm::mat4 assimp_mat_to_gml_mat(const aiMatrix4x4 &from) {
  return glm::mat4{
      {from.a1, from.b1, from.c1, from.d1},
      {from.a2, from.b2, from.c2, from.d2},
      {from.a3, from.b3, from.c3, from.d3},
      {from.a4, from.b4, from.c4, from.d4},
  };
}

inline glm::vec3 assimp_vec3_to_glm_vec3(const aiVector3D &vec) {
  return glm::vec3{vec.x, vec.y, vec.z};
}

inline glm::quat assimp_quat_to_glm_quat(const aiQuaternion &pOrientation) {
  return glm::quat{pOrientation.w, pOrientation.x, pOrientation.y,
                   pOrientation.z};
}

} // namespace figine::core::_object_internal
