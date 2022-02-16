#pragma once

#include "entity.hpp"
#include "figine/core/shader/shader_profile.hpp"

namespace figine::builtin::shader {

constexpr const uint8_t phong_vs[] = R"(
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

constexpr const uint8_t phong_fs[] = R"(
#version 330 core

struct material_t {
    float shininess;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct light_t {
    vec3 position;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 texture_coordinate;

out vec4 frag_color;

uniform vec3 view_pos;
uniform material_t material;
uniform light_t light;
uniform sampler2D texture_diffuse1;

void main() {
    vec3 norm = normalize(normal);
    vec3 view_direction = normalize(view_pos - frag_pos);
    vec3 light_direction = normalize(frag_pos - light.position);
    vec3 reflect_direction = reflect(light_direction, norm);

    // ambient
    vec3 ambient = light.ambient_color * material.ambient_color;

    // diffuse 
    float diff = max(dot(norm, -light_direction), 0.0);
    vec3 diffuse = diff * light.diffuse_color * material.diffuse_color;

    // specular
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = spec * light.specular_color * material.specular_color;

    frag_color = vec4((ambient + diffuse + specular) * texture(texture_diffuse1, texture_coordinate).xyz, 1.0);
}
)";

class phong_profile_t final : public figine::core::shader_profile_t {
public:
  inline phong_profile_t() = default;
  inline phong_profile_t(const material_t &material, const light_t &light)
      : material(material), light(light) {}

  material_t material;
  light_t light;

  inline const figine::core::meta_profile_t &meta() const override {
    figine::core::shader_profile_t::meta();

    _meta.insert("light.position", light.position);
    _meta.insert("light.ambient_color", light.ambient_color);
    _meta.insert("light.diffuse_color", light.diffuse_color);
    _meta.insert("light.specular_color", light.specular_color);

    _meta.insert("material.shininess", material.shininess);
    _meta.insert("material.ambient_color", material.ambient_color);
    _meta.insert("material.diffuse_color", material.diffuse_color);
    _meta.insert("material.specular_color", material.specular_color);

    return _meta;
  }
};

class phong_shader_t final : public figine::core::shader_t<phong_profile_t> {
public:
  phong_shader_t(const material_t &material, const light_t &light)
      : figine::core::shader_t<phong_profile_t>(
            phong_vs, phong_fs,
            std::make_shared<phong_profile_t>(material, light)) {}

  phong_shader_t(std::shared_ptr<phong_profile_t> profile = nullptr)
      : figine::core::shader_t<phong_profile_t>(phong_vs, phong_fs, profile) {
    if (this->profile == nullptr) {
      this->profile = std::make_shared<phong_profile_t>();
    }
  }
};

} // namespace figine::builtin::shader
