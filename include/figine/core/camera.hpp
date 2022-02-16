#pragma once

#include "figine/dep.hpp"

namespace figine::core {

enum class camera_movement_t { FORWARD, BACKWARD, LEFT, RIGHT };

class camera_t final {
public:
  camera_t(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
           const glm::vec3 &world_up = glm::vec3(0.0f, 1.0f, 0.0f));

  glm::mat4 view_matrix() const;

  void process_keyboard(camera_movement_t direction, float_t delta_time);
  void process_mouse_movement(float x_offset, float y_offset,
                              GLboolean constrain_pitch = true);
  void process_mouse_scroll(float y_offset);

  void lock(glm::vec3 target);
  void unlock();
  void update();

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;

  float pitch;
  float yaw;

  float speed;
  float mouse_sensitivity;
  float zoom;

private:
  glm::vec3 _target;
  bool _lock = false;
};

} // namespace figine::core