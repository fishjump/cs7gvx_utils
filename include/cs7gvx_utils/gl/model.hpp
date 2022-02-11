#pragma once

#include "camera.hpp"
#include "gl.hpp"
#include "mesh.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

#include <assimp/scene.h>

namespace cs7gvx_utils::gl {

class model_if {
public:
  glm::mat4 transform = glm::mat4(1);
  const cs7gvx_utils::gl::camera_t *camera = nullptr;
  const cs7gvx_utils::gl::shader_t *shader = nullptr;

  virtual void init() = 0;
  virtual void loop() = 0;

  glm::mat4 translate(const glm::vec3 &v);
  static glm::mat4 translate(const glm::vec3 &v, const glm::mat4 &relative_to);

  glm::mat4 scale(const glm::vec3 &v);
  static glm::mat4 scale(const glm::vec3 &v, const glm::mat4 &relative_to);

  glm::mat4 rotate_around(float degree, const glm::vec3 &axis);
  static glm::mat4 rotate_around(float degree, const glm::vec3 &axis,
                                 const glm::mat4 &relative_to);

  glm::mat4 rotate(const glm::vec3 &eulers, bool quaternion);
  static glm::mat4 rotate(const glm::vec3 &eulers, bool quaternion,
                          const glm::mat4 &relative_to);
};

class model_t : public model_if {
public:
  model_t(const std::string &path, const cs7gvx_utils::gl::shader_t *shader,
          const cs7gvx_utils::gl::camera_t *camera,
          float aspect_ratio = (float)800 / (float)600,
          bool gamma_correction = false);

  std::shared_ptr<gl::shader_profile_t> profile();

  virtual void init() override;
  void loop() final;

  virtual void update();

private:
  float _aspect_ratio;
  bool _gamma_correction;

  std::string _file;
  std::string _dir;
  std::vector<mesh_t> _meshes;
  std::unordered_map<std::string, texture_t> _textures_cache;

  void draw();
  void update_profile();

  cs7gvx_utils::common::result_t<> load(const std::string &path);
  void process_node(aiNode *node, const aiScene *scene);
  mesh_t process_mesh(aiMesh *mesh, const aiScene *scene);
  std::vector<gl::texture_t>
  load_material_textures(aiMaterial *mat, aiTextureType type,
                         const std::string &typeName);
};

} // namespace cs7gvx_utils::gl
