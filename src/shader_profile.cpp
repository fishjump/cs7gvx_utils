#include <cs7gvx_utils/gl/shader_profile.hpp>

cs7gvx_utils::gl::shader_profile_t::shader_profile_t() {}
cs7gvx_utils::gl::shader_profile_t::shader_profile_t(const material_t &material,
                                                     const light_t &light)
    : material(material), light(light) {}

cs7gvx_utils::gl::phong_profile_t::phong_profile_t() {}
cs7gvx_utils::gl::phong_profile_t::phong_profile_t(const material_t &material,
                                                   const light_t &light)
    : shader_profile_t(material, light) {}

cs7gvx_utils::gl::gooch_profile_t::gooch_profile_t() {}
cs7gvx_utils::gl::gooch_profile_t::gooch_profile_t(const material_t &material,
                                                   const light_t &light)
    : shader_profile_t(material, light) {}

const cs7gvx_utils::gl::meta_profile_t &
cs7gvx_utils::gl::shader_profile_t::meta() {
  cs7gvx_utils::gl::insert(_meta, "view_pos", view_pos);

  cs7gvx_utils::gl::insert(_meta, "light.position", light.position);
  cs7gvx_utils::gl::insert(_meta, "light.ambient_color", light.ambient_color);
  cs7gvx_utils::gl::insert(_meta, "light.diffuse_color", light.diffuse_color);
  cs7gvx_utils::gl::insert(_meta, "light.specular_color", light.specular_color);

  cs7gvx_utils::gl::insert(_meta, "material.shininess", material.shininess);
  cs7gvx_utils::gl::insert(_meta, "material.ambient_color",
                           material.ambient_color);
  cs7gvx_utils::gl::insert(_meta, "material.diffuse_color",
                           material.diffuse_color);
  cs7gvx_utils::gl::insert(_meta, "material.specular_color",
                           material.specular_color);

  cs7gvx_utils::gl::insert(_meta, "projection", projection);
  cs7gvx_utils::gl::insert(_meta, "view", view);
  cs7gvx_utils::gl::insert(_meta, "model", model);

  return _meta;
}

const cs7gvx_utils::gl::meta_profile_t &
cs7gvx_utils::gl::phong_profile_t::meta() {
  shader_profile_t::meta();

  return _meta;
}

const cs7gvx_utils::gl::meta_profile_t &
cs7gvx_utils::gl::gooch_profile_t::meta() {
  shader_profile_t::meta();

  cs7gvx_utils::gl::insert(_meta, "a", a);
  cs7gvx_utils::gl::insert(_meta, "b", b);
  cs7gvx_utils::gl::insert(_meta, "k_blue", k_blue);
  cs7gvx_utils::gl::insert(_meta, "k_yellow", k_yellow);

  return _meta;
}

const cs7gvx_utils::gl::meta_profile_t &
cs7gvx_utils::gl::cook_torrance_profile_t::meta() {
  shader_profile_t::meta();

  return _meta;
}