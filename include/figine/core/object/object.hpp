#pragma once

class shader_if;

#include "figine/core/camera.hpp"
#include "figine/core/shader/shader_if.hpp"
#include "mesh.hpp"

#include <string>
#include <type_traits>

namespace figine::core {

class object_if {
public:
  virtual void init() = 0;
  virtual void update() = 0;
  virtual void loop() = 0;

  static glm::mat4 translate(const glm::vec3 &v, const glm::mat4 &relative_to);
  static glm::mat4 scale(const glm::vec3 &v, const glm::mat4 &relative_to);
  static glm::mat4 rotate_around(float degree, const glm::vec3 &axis,
                                 const glm::mat4 &relative_to);
  static glm::mat4 rotate(const glm::vec3 &eulers, bool quaternion,
                          const glm::mat4 &relative_to);
};

class object_t : public object_if {
public:
  object_t(const std::string &file, const shader_if *shader,
           const camera_t *camera, bool gamma_correction = false);

  glm::mat4 transform;
  bool gamma_correction;

  const camera_t *camera;
  const shader_if *shader;

  void init() override;
  void update() override;
  void loop() override;

  glm::mat4 translate(const glm::vec3 &v);
  glm::mat4 scale(const glm::vec3 &v);
  glm::mat4 rotate_around(float degree, const glm::vec3 &axis);
  glm::mat4 rotate(const glm::vec3 &eulers, bool quaternion);

private:
  std::string _file;
  mutable std::vector<mesh_t> _meshes;
};

} // namespace figine::core