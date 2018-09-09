#pragma once

#include "src/shader_handler.hh"
#include <memory>

/** Note:
 * Pretty useless for now since, for the moment, a material
 * is just a container for a program.
 */
class Material {
  public:
    Material(std::shared_ptr<Program> program) { m_program = program; }
    ~Material();

    std::shared_ptr<Program> program() { return m_program; }

  private:
    std::shared_ptr<Program> m_program;
};
