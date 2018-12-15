#include "cube.hh"

Cube::Cube(std::string vs, std::string fs)
	: SceneNode("cube")
	, m_material({ vs, fs })
{
	m_mesh.upload_to_gpu();
}

Cube::~Cube()
{
	m_mesh.remove_from_gpu();
}

void Cube::setMaterialData(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model)
{
	m_uniforms.floats["time"] = ctx->t_time;
	m_uniforms.floats["dt"] = ctx->f_time;
	m_uniforms.mat4["view"] = ctx->activeCamera->view();
	m_uniforms.mat4["projection"] = ctx->perspective;
	m_uniforms.mat4["model"] = model;
}

void Cube::draw(std::shared_ptr<SceneContext> ctx, glm::mat4x4 model)
{
	glm::mat4 new_model = model * transform();
	setMaterialData(ctx, new_model);
	std::shared_ptr<MaterialData> data_ptr = std::make_shared<MaterialData>(std::move(m_uniforms));
	m_material.set_data(data_ptr);
	m_material.use();
	m_mesh.draw();
	m_material.release();

	SceneNode::draw(ctx, new_model);
}