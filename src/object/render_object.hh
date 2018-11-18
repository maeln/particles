#pragma once

#include <memory>

#include "src/object/drawable.hh"
#include "src/object/material/material.hh"
#include "src/object/mesh/mesh.hh"
#include "src/object/positionable.hh"

/// A fully drawable and positionnable object.
class RenderObject : public Drawable, public Positionable {
public:
	RenderObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
	{
		m_mesh = mesh;
		m_material = material;
	}

	~RenderObject() {}

	void set_mesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
	std::shared_ptr<Mesh> get_mesh() { return m_mesh; }
	void set_material(std::shared_ptr<Material> material) { m_material = material; }
	std::shared_ptr<Material> get_material() { return m_material; }

	void draw(std::shared_ptr<MaterialData> material_data)
	{
		m_material->use();
		m_material->set_data(material_data);
		m_mesh->draw();
	}

private:
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
};
