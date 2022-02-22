#include "figine/core/object.hpp"
#include "figine/logging.hpp"
#include "object_loader.hpp"

namespace figine::core {

void object_if::apply_uniform(const shader_if &shader) {
  shader.use();
  shader.set_uniform("view_pos", view_pos);
  shader.set_uniform("projection", projection);
  shader.set_uniform("view", view);
  shader.set_uniform("transform", transform);
};

object_if::object_if(const camera_t *camera)
    : camera(camera), view_pos(glm::vec3(1.0f)), projection(glm::mat4(1.0f)),
      view(glm::mat4(1.0f)), transform(glm::mat4(1.0f)){};

glm::mat4 object_if::translate(const glm::vec3 &v,
                               const glm::mat4 &relative_to) {
  return glm::translate(relative_to, v);
}

glm::mat4 object_if::scale(const glm::vec3 &v, const glm::mat4 &relative_to) {
  return glm::scale(relative_to, v);
}

glm::mat4 object_if::rotate_around(float degree, const glm::vec3 &axis,
                                   const glm::mat4 &relative_to) {
  return glm::rotate(relative_to, degree, axis);
}

glm::mat4 object_if::rotate(const glm::vec3 &eulers, bool quaternion,
                            const glm::mat4 &relative_to) {
  using namespace glm;

  // yaw, pitch, roll
  const float &a = eulers[2], &b = eulers[1], &y = eulers[0];

  if (!quaternion) {
    glm::mat3 r_x_yaw = {{cos(a), 0, sin(a)}, {0, 1, 0}, {-sin(a), 0, cos(a)}};
    glm::mat3 r_y_pitch = {
        {1, 0, 0}, {0, cos(b), -sin(b)}, {0, sin(b), cos(b)}};
    glm::mat3 r_z_roll = {{cos(y), -sin(y), 0}, {sin(y), cos(y), 0}, {0, 0, 1}};

    return relative_to * glm::mat4(r_z_roll * r_y_pitch * r_x_yaw);
  }

  float q0 = cos(a / 2) * cos(b / 2) * cos(y / 2) +
             sin(a / 2) * sin(b / 2) * sin(y / 2);
  float q1 = cos(a / 2) * sin(b / 2) * cos(y / 2) -
             sin(a / 2) * cos(b / 2) * sin(y / 2);
  float q2 = sin(a / 2) * cos(b / 2) * cos(y / 2) +
             cos(a / 2) * sin(b / 2) * sin(y / 2);
  float q3 = cos(a / 2) * cos(b / 2) * sin(y / 2) -
             sin(a / 2) * sin(b / 2) * cos(y / 2);

  glm::mat3 trans = {
      {
          1 - 2 * (glm::pow(q2, 2) + glm::pow(q3, 2)),
          2 * (q1 * q2 - q0 * q3),
          2 * (q1 * q3 + q0 * q2),
      },
      {
          2 * (q1 * q2 + q0 * q3),
          1 - 2 * (glm::pow(q1, 2) + glm::pow(q3, 2)),
          2 * (q2 * q3 - q0 * q1),
      },
      {
          2 * (q1 * q3 - q0 * q2),
          2 * (q2 * q3 + q0 * q1),
          1 - 2 * (glm::pow(q1, 2) + glm::pow(q2, 2)),
      },
  };

  return relative_to * glm::mat4(trans);
}

glm::mat4 object_if::translate(const glm::vec3 &v) {
  return object_if::translate(v, transform);
}

glm::mat4 object_if::scale(const glm::vec3 &v) {
  return object_if::scale(v, transform);
}

glm::mat4 object_if::rotate_around(float degree, const glm::vec3 &axis) {
  return object_if::rotate_around(degree, axis, transform);
}

glm::mat4 object_if::rotate(const glm::vec3 &eulers, bool quaternion) {
  return object_if::rotate(eulers, quaternion, transform);
}

object_t::object_t(const std::string &file, const camera_t *camera,
                   bool gamma_correction)
    : object_if(camera), gamma_correction(gamma_correction), _file(file) {}

void object_t::init() {
  if (!_object_internal::load(_meshes, bone_info_map, _file,
                              gamma_correction)) {
    LOG_ERR("failed to load meshes");
    return;
  }

  for (auto &mesh : _meshes) {
    mesh.init();
  }
}

void object_t::update() {
  view_pos = camera->position;
  projection =
      glm::perspective(glm::radians(camera->zoom),
                       figine::global::win_mgr::aspect_ratio(), 0.1f, 100.0f);
  view = camera->view_matrix();
  transform = transform;
}

void object_t::loop(const shader_if &shader) {
  update();
  apply_uniform(shader);

  for (const auto &mesh : _meshes) {
    mesh.draw(shader);
  }
}

void object_t::apply_uniform(const shader_if &shader) {
  object_if::apply_uniform(shader);
}

} // namespace figine::core
