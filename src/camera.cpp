#include <cs7gvx_utils/gl/camera.hpp>

#include <string>

cs7gvx_utils::gl::camera_t::camera_t(glm::vec3 position, glm::vec3 world_up,
                                     float pitch, float yaw)
    : _position(position), _front(glm::vec3(0.0f, 0.0f, -1.0f)),
      _world_up(world_up), _pitch(pitch), _yaw(yaw), _speed(SPEED),
      _mouse_sensitivity(SENSITIVITY), _zoom(ZOOM) {
  update_camera_vectors();
}

glm::mat4 cs7gvx_utils::gl::camera_t::view_matrix() const {
  if (_lock) {
    return glm::lookAt(_position, _target, _up);
  }

  return glm::lookAt(_position, _position + _front, _up);
}

void cs7gvx_utils::gl::camera_t::lock(glm::vec3 target) {
  _lock = true;
  _target = target;
}
void cs7gvx_utils::gl::camera_t::unlock() { _lock = false; }

void cs7gvx_utils::gl::camera_t::process_keyboard(camera_movement_t direction,
                                                  float_t delta_time) {
  float velocity = _speed * delta_time;
  switch (direction) {
  case camera_movement_t::FORWARD:
    if (false) {
      _position =
          glm::vec4(_position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), {1.0f, 0.0f, 0.0f});
    } else {
      _position += _front * velocity;
    }
    break;
  case camera_movement_t::BACKWARD:
    if (false) {
      _position =
          glm::vec4(_position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), {1.0f, 0.0f, 0.0f});
    } else {
      _position -= _front * velocity;
    }
    break;
  case camera_movement_t::LEFT:
    if (_lock) {
      _position =
          glm::vec4(_position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), {0.0f, 1.0f, 0.0f});
    } else {
      _position -= _right * velocity;
    }
    break;
  case camera_movement_t::RIGHT:
    if (_lock) {
      _position =
          glm::vec4(_position, 0) *
          glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), {0.0f, 1.0f, 0.0f});
    } else {
      _position += _right * velocity;
    }
    break;
  }
}

void cs7gvx_utils::gl::camera_t::process_mouse_movement(
    float x_offset, float y_offset, GLboolean constrain_pitch) {
  x_offset *= _mouse_sensitivity;
  y_offset *= _mouse_sensitivity;

  _yaw += x_offset;
  _pitch += y_offset;

  if (constrain_pitch) {
    if (_pitch > 89.0f) {
      _pitch = 89.0f;
    } else if (_pitch < -89.0f) {
      _pitch = -89.0f;
    }
  }

  update_camera_vectors();
}

void cs7gvx_utils::gl::camera_t::process_mouse_scroll(float y_offset) {
  _zoom -= y_offset;
  if (_zoom < 1.0f) {
    _zoom = 1.0f;
  } else if (_zoom > 45.0f) {
    _zoom = 45.0f;
  }
}

void cs7gvx_utils::gl::camera_t::update_camera_vectors() {
  glm::vec3 front = {cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
                     sin(glm::radians(_pitch)),
                     sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))};
  _front = glm::normalize(front);
  _right = glm::normalize(glm::cross(_front, _world_up));
  _up = glm::normalize(glm::cross(_right, _front));
}

glm::vec3 &cs7gvx_utils::gl::camera_t::position() { return _position; }
glm::vec3 &cs7gvx_utils::gl::camera_t::front() { return _front; }
glm::vec3 &cs7gvx_utils::gl::camera_t::up() { return _up; }
glm::vec3 &cs7gvx_utils::gl::camera_t::right() { return _right; }
glm::vec3 &cs7gvx_utils::gl::camera_t::world_up() { return _world_up; }

float cs7gvx_utils::gl::camera_t::yaw() const { return _yaw; }
float cs7gvx_utils::gl::camera_t::pitch() const { return _pitch; }

float cs7gvx_utils::gl::camera_t::speed() const { return _speed; }
float cs7gvx_utils::gl::camera_t::mouse_sensitivity() const {
  return _mouse_sensitivity;
}

float cs7gvx_utils::gl::camera_t::zoom() const { return _zoom; }
