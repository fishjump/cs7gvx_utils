#include <cs7gvx_utils/gl/camera.hpp>

#include <string>

cs7gvx_utils::gl::camera_t::camera_t(glm::vec3 position, glm::vec3 world_up,
                                     float pitch, float yaw)
    : position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)),
      world_up(world_up), pitch(pitch), yaw(yaw), speed(SPEED),
      mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
  update_camera_vectors();
}

glm::mat4 cs7gvx_utils::gl::camera_t::view_matrix() const {
  if (_lock) {
    return glm::lookAt(position, _target, up);
  }

  return glm::lookAt(position, position + front, up);
}

void cs7gvx_utils::gl::camera_t::lock(glm::vec3 target) {
  _lock = true;
  _target = target;
}
void cs7gvx_utils::gl::camera_t::unlock() { _lock = false; }

void cs7gvx_utils::gl::camera_t::process_keyboard(camera_movement_t direction,
                                                  float_t delta_time) {
  float velocity = speed * delta_time;
  switch (direction) {
  case camera_movement_t::FORWARD:
    if (_lock) {
      position =
          glm::vec4(position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), {1.0f, 0.0f, 0.0f});
    } else {
      position += glm::vec3{0, 0, -1} * velocity;
    }
    break;
  case camera_movement_t::BACKWARD:
    if (_lock) {
      position =
          glm::vec4(position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), {1.0f, 0.0f, 0.0f});
    } else {
      position += glm::vec3{0, 0, 1} * velocity;
    }
    break;
  case camera_movement_t::LEFT:
    if (_lock) {
      position =
          glm::vec4(position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), {0.0f, 1.0f, 0.0f});
    } else {
      position += glm::vec3{0, 0, -1} * velocity;
    }
    break;
  case camera_movement_t::RIGHT:
    if (_lock) {
      position =
          glm::vec4(position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), {0.0f, 1.0f, 0.0f});
    } else {
      position += glm::vec3{0, 0, 1} * velocity;
    }
    break;
  }
}

void cs7gvx_utils::gl::camera_t::process_mouse_movement(
    float x_offset, float y_offset, GLboolean constrainpitch) {
  x_offset *= mouse_sensitivity;
  y_offset *= mouse_sensitivity;

  yaw += x_offset;
  pitch += y_offset;

  if (constrainpitch) {
    if (pitch > 89.0f) {
      pitch = 89.0f;
    } else if (pitch < -89.0f) {
      pitch = -89.0f;
    }
  }

  update_camera_vectors();
}

void cs7gvx_utils::gl::camera_t::process_mouse_scroll(float y_offset) {
  zoom -= y_offset;
  if (zoom < 1.0f) {
    zoom = 1.0f;
  } else if (zoom > 45.0f) {
    zoom = 45.0f;
  }
}

void cs7gvx_utils::gl::camera_t::update_camera_vectors() {
  glm::vec3 front = {cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                     sin(glm::radians(pitch)),
                     sin(glm::radians(yaw)) * cos(glm::radians(pitch))};
  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}
