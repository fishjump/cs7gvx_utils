#pragma once

#include "assimp_to_glm_cvt.hpp"
#include "figine/figine.hpp"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace figine::core::_object_internal {

inline GLuint texture_from_file(const std::string &file, const std::string &dir,
                                bool gamma) {
  const std::string filename = dir + "/" + file;

  GLuint texture_id;
  glGenTextures(1, &texture_id);

  int width = 0, height = 0, nr_components = 0;
  uint8_t *data =
      stbi_load(filename.c_str(), &width, &height, &nr_components, 0);
  defer(stbi_image_free(data));
  if (!data) {
    LOG_ERR("texture failed to load at path: %s", filename.c_str());
    return 0;
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

  return texture_id;
}

inline std::vector<figine::core::texture_t>
load_material_textures(const std::string &cwd, aiMaterial *mat,
                       aiTextureType type, const std::string &type_name,
                       bool gamma_correction) {
  static std::unordered_map<std::string, figine::core::texture_t>
      _textures_cache;

  std::vector<figine::core::texture_t> textures;
  for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
    aiString texture_file;
    mat->GetTexture(type, i, &texture_file);

    if (auto texture_cache = _textures_cache.find(texture_file.C_Str());
        texture_cache != _textures_cache.end()) {
      textures.push_back(texture_cache->second);
      continue;
    }

    GLuint texture_id =
        texture_from_file(texture_file.C_Str(), cwd, gamma_correction);
    if (texture_id == 0) {
      LOG_ERR("failed to load texture: %s", texture_file.C_Str());
      continue;
    }

    figine::core::texture_t texture = {
        .id = texture_id,
        .type = type_name,
        .path = texture_file.C_Str(),
    };
    textures.push_back(texture);
    _textures_cache.insert({texture.path, texture});
  }

  return textures;
}

inline void read_bone_weight_for_vertices(
    std::vector<vertex_t> &vertices,
    std::unordered_map<std::string, bone_info_t> &bone_info_map, aiMesh *mesh) {
  for (int i = 0; i < mesh->mNumBones; ++i) {
    std::string bone_name = mesh->mBones[i]->mName.C_Str();
    if (bone_info_map.find(bone_name) == bone_info_map.end()) {
      bone_info_map[bone_name] = bone_info_t{.id = (int)bone_info_map.size()};
    }
    bone_info_map[bone_name].offset =
        assimp_mat_to_gml_mat(mesh->mBones[i]->mOffsetMatrix);

    int bone_id = bone_info_map[bone_name].id;
    auto weights = mesh->mBones[i]->mWeights;
    for (int weight_index = 0, num_of_weights = mesh->mBones[i]->mNumWeights;
         weight_index < num_of_weights; weight_index++) {
      int vertex_id = weights[weight_index].mVertexId;
      float weight = weights[weight_index].mWeight;
      assert(vertex_id <= vertices.size());
      vertices[vertex_id].set_weight(bone_id, weight);
    }
  }
}

inline figine::core::mesh_t
load_mesh(std::unordered_map<std::string, bone_info_t> &bone_info_map,
          const std::string &cwd, aiMesh *mesh, const aiScene *scene,
          bool gamma_correction) {
  std::vector<figine::core::vertex_t> vertices;
  std::vector<uint32_t> indices;
  std::vector<figine::core::texture_t> textures;

  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    figine::core::vertex_t vertex{
        .position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                     mesh->mVertices[i].z},
    };

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

    vertex.init_bones();
    vertices.push_back(vertex);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<figine::core::texture_t> diffuse_maps =
      load_material_textures(cwd, material, aiTextureType_DIFFUSE,
                             "texture_diffuse", gamma_correction);
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

  std::vector<figine::core::texture_t> specular_maps =
      load_material_textures(cwd, material, aiTextureType_SPECULAR,
                             "texture_specular", gamma_correction);
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

  std::vector<figine::core::texture_t> normal_maps = load_material_textures(
      cwd, material, aiTextureType_HEIGHT, "texture_normal", gamma_correction);
  textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

  std::vector<figine::core::texture_t> height_maps = load_material_textures(
      cwd, material, aiTextureType_AMBIENT, "texture_height", gamma_correction);
  textures.insert(textures.end(), height_maps.begin(), height_maps.end());

  read_bone_weight_for_vertices(vertices, bone_info_map, mesh);

  return figine::core::mesh_t(vertices, indices, textures);
}

inline void
load_node(std::vector<figine::core::mesh_t> &meshes,
          std::unordered_map<std::string, bone_info_t> &bone_info_map,
          const std::string &cwd, aiNode *node, const aiScene *scene,
          bool gamma_correction) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(
        load_mesh(bone_info_map, cwd, mesh, scene, gamma_correction));
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    load_node(meshes, bone_info_map, cwd, node->mChildren[i], scene,
              gamma_correction);
  }
}

inline bool load(std::vector<figine::core::mesh_t> &meshes,
                 std::unordered_map<std::string, bone_info_t> &bone_info_map,
                 const std::string &path, bool gamma_correction) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !scene->mRootNode) {
    LOG_ERR("%s", importer.GetErrorString());
    return false;
  }

  const std::string cwd = path.substr(0, path.find_last_of('/'));

  load_node(meshes, bone_info_map, cwd, scene->mRootNode, scene,
            gamma_correction);

  return true;
}

} // namespace figine::core::_object_internal