#pragma once

#include "figine/core/shader.hpp"

namespace figine::builtin::shader {

constexpr uint8_t skybox_vs[] = R"(
#version 330 core

layout(location = 0) in vec3 pos_in;

out vec3 texture_coordinate;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main() {
  texture_coordinate = pos_in;
  vec3 frag_pos = vec3(transform * vec4(pos_in, 1.0));

  gl_Position = projection * view * vec4(frag_pos, 1.0);
}
)";

constexpr uint8_t skybox_fs[] = R"(
#version 330 core

out vec4 frag_color;

in vec3 texture_coordinate;

uniform samplerCube skybox;

void main() {
  frag_color = texture(skybox, texture_coordinate);
}
)";

class skybox_shader final
    : public figine::core::shader_t<figine::core::shader_profile_t> {
  skybox_shader(
      std::shared_ptr<figine::core::shader_profile_t> profile = nullptr)
      : figine::core::shader_t<figine::core::shader_profile_t>(
            skybox_vs, skybox_fs, profile) {
    if (this->profile == nullptr) {
      this->profile = std::make_shared<figine::core::shader_profile_t>();
    }
  }
};

} // namespace figine::builtin::shader
