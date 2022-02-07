#pragma once

#include "camera.hpp"
#include "gl.hpp"
#include "mesh.hpp"

#include <unordered_map>
#include <vector>

#include <assimp/scene.h>

namespace cs7gvx_utils::gl {

class model_t {
public:
  model_t(const std::string &path, cs7gvx_utils::gl::shader_t *shader,
          cs7gvx_utils::gl::camera_t *camera,
          float aspect_ratio = (float)800 / (float)600,
          bool gamma_correction = false);

  glm::mat4 &transform_mat();
  cs7gvx_utils::gl::shader_t &shader();
  std::shared_ptr<gl::shader_profile_t> profile();

  virtual void init();
  virtual void update();
  void loop();
  void bind_camera(cs7gvx_utils::gl::camera_t *camera);

  glm::mat4 translate(const glm::vec3 &v);
  glm::mat4 scale(const glm::vec3 &v);
  glm::mat4 rotate(float degree, const glm::vec3 &axis);
  glm::mat4 rotate(float roll, float pitch, float yaw, bool quaternion);
  static glm::mat4 rotate(float degree, const glm::vec3 &axis,
                          const glm::mat4 &relative);
  static glm::mat4 rotate(float roll, float pitch, float yaw, bool quaternion,
                          const glm::mat4 &relative);

protected:
  glm::mat4 _transform_mat;

private:
  float _aspect_ratio;
  bool _gamma_correction;

  cs7gvx_utils::gl::camera_t *_camera;
  cs7gvx_utils::gl::shader_t *_shader;

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
