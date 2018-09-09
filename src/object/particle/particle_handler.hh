#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <random>
#include <vector>

#include "src/graph/scene/scene_node.hh"
#include "src/shader_handler.hh"

class ParticleHandler : public SceneNode {
  public:
    ParticleHandler(GLuint nb_particule, float ttl_particule, float delta_ttl, glm::vec3 start_point, bool random_colour,
                    glm::vec3 base_colour);
    ~ParticleHandler();

    void update_particules(float time, float dt, float speed_factor = 1.f);
    void draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model);

    void set_colour(glm::vec3 col);
    void set_random_colour(bool r);
    bool is_color_random();

  private:
    GLuint m_vbo_pos;
    GLuint m_vbo_vel;
    GLuint m_vbo_ttl;
    GLuint m_vao;

    GLuint m_max_part;
    float m_ttl;
    float m_dttl;
    glm::vec3 m_pstart;
    GLuint m_texture;

    bool m_rand_colour;
    glm::vec3 m_base_colour;

    std::default_random_engine m_randgen;
    std::uniform_real_distribution<double> m_uniform;

    std::vector<float> m_part_pos;
    std::vector<float> m_part_vel;
    std::vector<float> m_colour;
    std::vector<float> m_part_ttl;

    ShaderHandler &m_shaders = ShaderHandler::instance();
    std::shared_ptr<Program> m_program;
    std::shared_ptr<Program> m_compute;

    float *m_data;
};
