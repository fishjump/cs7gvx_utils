#define STB_IMAGE_IMPLEMENTATION

#include <cs7gvx_utils/gl/model.hpp>

#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <boost/filesystem.hpp>
#include <cs7gvx_utils/gl.hpp>

#include <cs7gvx_utils/common.hpp>

namespace {

cs7gvx_utils::common::result_t<GLuint>
texture_from_file(const std::string &file, const std::string &dir, bool gamma) {
  const std::string filename = cs7gvx_utils::common::make_str(dir, '/', file);

  GLuint texture_id;
  glGenTextures(1, &texture_id);

  int width = 0, height = 0, nr_components = 0;
  uint8_t *data =
      stbi_load(filename.c_str(), &width, &height, &nr_components, 0);
  defer(stbi_image_free(data));
  if (!data) {
    std::string err = cs7gvx_utils::common::make_str(
        "Texture failed to load at path: ", filename);
    LOG_ERR(err);
    return {0, err};
  }

  GLenum format;
  switch (nr_components) {
  case 1:
    format = GL_RED;
    break;
  case 3:
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  }

  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return {texture_id, std::nullopt};
}

} // namespace

cs7gvx_utils::gl::model_t::model_t(const std::string &path,
                                   cs7gvx_utils::gl::shader_t *shader,
                                   const cs7gvx_utils::gl::camera_t *camera,
                                   float aspect_ratio, bool gamma_correction)
    : _transform_mat(glm::mat4(1.0f)), _aspect_ratio(aspect_ratio),
      _gamma_correction(gamma_correction), _camera(camera), _shader(shader),
      _file(path) {}

void cs7gvx_utils::gl::model_t::init() {
  auto res = load(_file);
  if (res.err != std::nullopt) {
    LOG_ERR(res.err.value());
    return;
  }
}
void cs7gvx_utils::gl::model_t::update() {}

void cs7gvx_utils::gl::model_t::loop() {
  update();
  update_profile();
  draw();
}

const glm::mat4 &cs7gvx_utils::gl::model_t::transform_mat() const {
  return _transform_mat;
}

std::shared_ptr<cs7gvx_utils::gl::shader_profile_t>
cs7gvx_utils::gl::model_t::profile() {
  return _shader->profile();
}

glm::mat4 cs7gvx_utils::gl::model_t::translate(const glm::vec3 &v) {
  return glm::translate(_transform_mat, v);
}

glm::mat4 cs7gvx_utils::gl::model_t::scale(const glm::vec3 &v) {
  return glm::scale(_transform_mat, v);
}

glm::mat4 cs7gvx_utils::gl::model_t::rotate(float degree,
                                            const glm::vec3 &axis) {
  return glm::rotate(_transform_mat, glm::radians(degree), axis);
}

glm::mat4 cs7gvx_utils::gl::model_t::rotate(float roll, float pitch, float yaw,
                                            bool _quaternion) {
  using namespace glm;
  float &a = yaw, &b = pitch, &y = roll;
  if (_quaternion) {
    glm::mat3 r_x_yaw = {{cos(a), 0, sin(a)}, {0, 1, 0}, {-sin(a), 0, cos(a)}};
    glm::mat3 r_y_pitch = {
        {1, 0, 0}, {0, cos(b), -sin(b)}, {0, sin(b), cos(b)}};
    glm::mat3 r_z_roll = {{cos(y), -sin(y), 0}, {sin(y), cos(y), 0}, {0, 0, 1}};

    return glm::mat4(r_z_roll * r_y_pitch * r_x_yaw);
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

  return glm::mat4(trans);
};

void cs7gvx_utils::gl::model_t::draw() {
  _shader->use();
  _shader->set_profile();

  for (const auto &mesh : _meshes) {
    mesh.draw(*_shader);
  }
}

void cs7gvx_utils::gl::model_t::update_profile() {
  _shader->profile()->view_pos = _camera->position();

  _shader->profile()->projection = glm::perspective(
      glm::radians(_camera->zoom()), _aspect_ratio, 0.1f, 100.0f);
  _shader->profile()->view = _camera->view_matrix();
  _shader->profile()->model = _transform_mat;
}

cs7gvx_utils::gl::shader_t &cs7gvx_utils::gl::model_t::shader() {
  return *_shader;
}

cs7gvx_utils::common::result_t<>
cs7gvx_utils::gl::model_t::load(const std::string &path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !scene->mRootNode) {
    const std::string err = importer.GetErrorString();
    LOG_ERR(err);
    return {cs7gvx_utils::common::none_v, err};
  }

  _dir = path.substr(0, path.find_last_of('/'));

  process_node(scene->mRootNode, scene);

  return {cs7gvx_utils::common::none_v, std::nullopt};
}

void cs7gvx_utils::gl::model_t::process_node(aiNode *node,
                                             const aiScene *scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    _meshes.push_back(process_mesh(mesh, scene));
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
}

cs7gvx_utils::gl::mesh_t
cs7gvx_utils::gl::model_t::process_mesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<gl::vertex_t> vertices;
  std::vector<uint32_t> indices;
  std::vector<gl::texture_t> textures;

  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    cs7gvx_utils::gl::vertex_t vertex;

    vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                       mesh->mVertices[i].z};

    if (mesh->HasNormals()) {
      vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                       mesh->mNormals[i].z};
    }

    if (mesh->mTextureCoords[0]) {
      vertex.texture_coordinate = {mesh->mTextureCoords[0][i].x,
                                   mesh->mTextureCoords[0][i].y};
      vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y,
                        mesh->mTangents[i].z};
      vertex.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y,
                          mesh->mBitangents[i].z};
    } else {
      vertex.texture_coordinate = {0.0f, 0.0f};
    }

    vertices.push_back(vertex);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<gl::texture_t> diffuse_maps = load_material_textures(
      material, aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

  std::vector<gl::texture_t> specular_maps = load_material_textures(
      material, aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

  std::vector<gl::texture_t> normal_maps =
      load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
  textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

  std::vector<gl::texture_t> height_maps =
      load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
  textures.insert(textures.end(), height_maps.begin(), height_maps.end());

  return mesh_t(vertices, indices, textures);
}

std::vector<cs7gvx_utils::gl::texture_t>
cs7gvx_utils::gl::model_t::load_material_textures(aiMaterial *mat,
                                                  aiTextureType type,
                                                  const std::string &typeName) {
  std::vector<gl::texture_t> textures;
  for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    if (auto texture_cache = _textures_cache.find(str.C_Str());
        texture_cache != _textures_cache.end()) {
      textures.push_back(texture_cache->second);
      continue;
    }

    auto res = texture_from_file(str.C_Str(), _dir, _gamma_correction);
    if (res.err != std::nullopt) {
      LOG_ERR(res.err.value());
      continue;
    }

    cs7gvx_utils::gl::texture_t texture = {
        .id = res.result,
        .type = typeName,
        .path = str.C_Str(),
    };
    textures.push_back(texture);
    _textures_cache.insert({texture.path, texture});
  }
  return textures;
}