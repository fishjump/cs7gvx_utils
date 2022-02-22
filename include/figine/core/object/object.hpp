#pragma once

#include "bone.hpp"
#include "mesh.hpp"

#include "figine/core/camera.hpp"
#include "figine/core/shader/shader_if.hpp"

#include <string>
#include <type_traits>
#include <unordered_map>

namespace figine::core {

class object_if {
public:
  object_if(const camera_t *camera);

  const camera_t *camera;

  glm::vec3 view_pos;
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 transform;

  virtual void init() = 0;
  virtual void update() = 0;
  virtual void apply_uniform(const shader_if &shader);
  virtual void loop(const shader_if &shader) = 0;

  static glm::mat4 translate(const glm::vec3 &v, const glm::mat4 &relative_to);
  static glm::mat4 scale(const glm::vec3 &v, const glm::mat4 &relative_to);
  static glm::mat4 rotate_around(float degree, const glm::vec3 &axis,
                                 const glm::mat4 &relative_to);
  static glm::mat4 rotate(const glm::vec3 &eulers, bool quaternion,
                          const glm::mat4 &relative_to);

  glm::mat4 translate(const glm::vec3 &v);
  glm::mat4 scale(const glm::vec3 &v);
  glm::mat4 rotate_around(float degree, const glm::vec3 &axis);
  glm::mat4 rotate(const glm::vec3 &eulers, bool quaternion);
};

class object_t : public object_if {
public:
  object_t(const std::string &file, const camera_t *camera,
           bool gamma_correction = false);

  bool gamma_correction;

  std::unordered_map<std::string, bone_info_t> bone_info_map;

  void init() override;
  void update() override;
  void loop(const shader_if &shader) override;
  void apply_uniform(const shader_if &shader) override;

private:
  std::string _file;
  mutable std::vector<mesh_t> _meshes;
};

} // namespace figine::core