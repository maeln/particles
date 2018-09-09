#pragma once

#include <memory>

#include "src/drawable/drawable.hh"
#include "src/drawable/material/material.hh"
#include "src/drawable/mesh/mesh.hh"

class Object : public Drawable {
  public:
    Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat) {
	m_mesh = mesh;
	m_material = mat;
    }
    ~Object();

  private:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
};