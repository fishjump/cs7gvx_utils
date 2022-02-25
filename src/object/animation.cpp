#include "figine/core/object/animation.hpp"
#include "assimp_to_glm_cvt.hpp"
#include "figine/logging.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtx/string_cast.hpp>

namespace figine::core {

animation_t::animation_t(const std::string &file, object_t &object) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(file, aiProcess_Triangulate);
  assert(scene && scene->mRootNode);

  auto animation = scene->mAnimations[0];
  duration = animation->mDuration;
  ticks_per_second = animation->mTicksPerSecond;

  read_heirarchy_data(root, scene->mRootNode);
  read_missing_bones(animation, object);
}

bone_t *animation_t::find_bone(const std::string &name) {
  auto iter =
      std::find_if(bones.begin(), bones.end(),
                   [&](const bone_t &bone_t) { return bone_t.name == name; });

  if (iter == bones.end()) {
    return nullptr;
  } else {
    return &(*iter);
  }
}

void animation_t::read_missing_bones(const aiAnimation *animation,
                                     object_t &object) {
  int size = animation->mNumChannels;
  auto &bone_info_map = object.bone_info_map;

  for (int i = 0; i < size; i++) {
    auto channel = animation->mChannels[i];
    std::string boneName = channel->mNodeName.data;

    if (bone_info_map.find(boneName) == bone_info_map.end()) {
      bone_info_map[boneName] = bone_info_t{.id = (int)bone_info_map.size()};
    }
    bones.push_back(bone_t(channel->mNodeName.data,
                           bone_info_map[channel->mNodeName.data].id, channel));
  }

  this->bone_info_map = bone_info_map;
}

void animation_t::read_heirarchy_data(bone_data_t &dest, const aiNode *src) {
  assert(src);

  dest.name = src->mName.data;
  dest.transform =
      _object_internal::assimp_mat_to_gml_mat(src->mTransformation);

  for (int i = 0; i < src->mNumChildren; i++) {
    bone_data_t new_data;
    read_heirarchy_data(new_data, src->mChildren[i]);
    dest.children.push_back(new_data);
  }
}

} // namespace figine::core