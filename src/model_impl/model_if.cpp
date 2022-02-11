#include <cs7gvx_utils/gl/model.hpp>

glm::mat4 cs7gvx_utils::gl::model_if::translate(const glm::vec3 &v) {
  return glm::translate(transform, v);
}

glm::mat4 cs7gvx_utils::gl::model_if::translate(const glm::vec3 &v,
                                                const glm::mat4 &relative_to) {
  return glm::translate(relative_to, v);
}

glm::mat4 cs7gvx_utils::gl::model_if::scale(const glm::vec3 &v) {
  return glm::scale(transform, v);
}

glm::mat4 cs7gvx_utils::gl::model_if::scale(const glm::vec3 &v,
                                            const glm::mat4 &relative_to) {
  return glm::scale(relative_to, v);
}

glm::mat4 cs7gvx_utils::gl::model_if::rotate_around(float degree,
                                                    const glm::vec3 &axis) {
  return glm::rotate(transform, degree, axis);
}

glm::mat4
cs7gvx_utils::gl::model_if::rotate_around(float degree, const glm::vec3 &axis,
                                          const glm::mat4 &relative_to) {
  return glm::rotate(relative_to, degree, axis);
}

glm::mat4 cs7gvx_utils::gl::model_if::rotate(const glm::vec3 &eulers,
                                             bool quaternion) {
  return rotate(eulers, quaternion, transform);
}

glm::mat4 cs7gvx_utils::gl::model_if::rotate(const glm::vec3 &eulers,
                                             bool quaternion,
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
