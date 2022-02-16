#pragma once

#include "figine/core/shader.hpp"

namespace figine::builtin::shader {

constexpr uint8_t naive_vs[] = R"(
#version 330 core

layout(location = 0) in vec3 pos_in;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texture_coordinate_in;

out vec3 frag_pos;
out vec3 normal;
out vec2 texture_coordinate;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    texture_coordinate = texture_coordinate_in;
    frag_pos = vec3(transform * vec4(pos_in, 1.0));
    normal = mat3(transpose(inverse(transform))) * normal_in;

    gl_Position = projection * view * vec4(frag_pos, 1.0);
}
)";

constexpr uint8_t naive_fs[] = R"(
#version 330 core

out vec4 frag_color;

void main() {
    frag_color = vec4(1);
}
)";

class naive_shader_t final
    : public figine::core::shader_t<figine::core::shader_profile_t> {
public:
  naive_shader_t(
      std::shared_ptr<figine::core::shader_profile_t> profile = nullptr)
      : figine::core::shader_t<figine::core::shader_profile_t>(
            naive_vs, naive_fs, profile) {
    if (this->profile == nullptr) {
      this->profile = std::make_shared<figine::core::shader_profile_t>();
    }
  }
};

} // namespace figine::builtin::shader