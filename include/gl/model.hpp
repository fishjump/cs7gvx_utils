#pragma once

#include "camera.hpp"
#include "gl.hpp"
#include "mesh.hpp"

#include <unordered_map>
#include <vector>

#include <assimp/scene.h>

namespace gl {

class model_t {
public:
  model_t(const std::string &path, gl::shader_t *shader,
          const gl::camera_t *camera, float aspect_ratio = 16 / 9.0,
          bool gamma_correction = false);

  const glm::mat4 &transform_mat() const;
  gl::shader_t &shader();
  std::shared_ptr<gl::shader_profile_t> profile();

  virtual void init();
  virtual void update();
  void loop();
  void bind_camera(const gl::camera_t *camera) { _camera = camera; }

  glm::mat4 translate(const glm::vec3 &v);
  glm::mat4 scale(const glm::vec3 &v);
  glm::mat4 rotate(float degree, const glm::vec3 &axis);
  glm::mat4 rotate(float roll, float pitch, float yaw, bool enable_gimbal_lock);

protected:
  glm::mat4 _transform_mat;

private:
  float _aspect_ratio;
  bool _gamma_correction;

  const gl::camera_t *_camera;
  gl::shader_t *_shader;

  std::string _file;
  std::string _dir;
  std::vector<mesh_t> _meshes;
  std::unordered_map<std::string, texture_t> _textures_cache;

  void draw();
  void update_profile();

  common::result_t<> load(const std::string &path);
  void process_node(aiNode *node, const aiScene *scene);
  mesh_t process_mesh(aiMesh *mesh, const aiScene *scene);
  std::vector<gl::texture_t>
  load_material_textures(aiMaterial *mat, aiTextureType type,
                         const std::string &typeName);
};

} // namespace gl
