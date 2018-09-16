#pragma once

#include "src/object/mesh/basic_mesh.hh"

const float quad_verts[] = {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
const float quad_tex_coords[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

class QuadMesh : public BasicMesh {
  public:
    QuadMesh()
        : BasicMesh(
              std::unique_ptr<std::vector<float>>(new std::vector<float>(quad_verts, quad_verts + sizeof(quad_verts) / sizeof(float))),
              std::unique_ptr<std::vector<float>>(
                  new std::vector<float>(quad_tex_coords, quad_tex_coords + sizeof(quad_tex_coords) + sizeof(float))),
              GL_TRIANGLES, 2){};
    ~QuadMesh(){};
};