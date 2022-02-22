#pragma once

#include "figine/core/camera.hpp"
#include "figine/core/object.hpp"
#include "figine/global.hpp"
#include "figine/logging.hpp"
#include "stb_image.h"

namespace figine::builtin::object {

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

class skybox_t : public figine::core::object_if {

public:
  inline skybox_t(const std::array<std::string, 6> &faces,
                  const figine::core::camera_t *camera,
                  bool gamma_correction = false)
      : figine::core::object_if(camera), gamma_correction(gamma_correction),
        _faces(faces) {}

  bool gamma_correction = false;
  glm::mat4 transform;

  inline void init() override {
    shader.build();

    static float vertices[] = {
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

        1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

    // model
    GLuint vbo = 0;
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);

    // texture
    glGenTextures(1, &_box_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _box_texture);

    int width, height, nr_components;
    for (size_t i = 0; i < _faces.size(); i++) {
      uint8_t *data =
          stbi_load(_faces[i].c_str(), &width, &height, &nr_components, 0);
      defer(stbi_image_free(data));
      if (data) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                     height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      } else {
        LOG_ERR("cubemap texture failed to load at path: %s",
                _faces[i].c_str());
      }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  inline void update() override {
    view_pos = camera->position;
    projection =
        glm::perspective(glm::radians(camera->zoom),
                         figine::global::win_mgr::aspect_ratio(), 0.1f, 100.0f);
    view = camera->view_matrix();
  }

  void loop(const figine::core::shader_if &shader) override {
    glDepthFunc(GL_LEQUAL);

    update();
    apply_uniform(shader);

    glBindVertexArray(_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _box_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
  }

  inline void loop() { loop(shader); }

private:
  class skybox_shader_t final : public figine::core::shader_if {
  public:
    inline skybox_shader_t() : figine::core::shader_if(skybox_vs, skybox_fs) {}
  };

  skybox_shader_t shader;
  GLuint _vao = 0, _box_texture = 0;
  std::array<std::string, 6> _faces;
};

} // namespace figine::builtin::object