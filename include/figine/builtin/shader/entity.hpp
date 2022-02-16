#pragma once

#include "../../dep.hpp"

namespace figine::builtin::shader {

struct material_t {
  GLfloat shininess;
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
};

struct light_t {
  glm::vec3 position;
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
};

} // namespace figine::builtin::shader