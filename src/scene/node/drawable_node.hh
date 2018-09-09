#pragma once

#include "positionable_node.hh"
#include "src/drawable/drawable.hh"

class DrawableNode : public PositionableNode, public Drawable {
  public:
    DrawableNode();
    ~DrawableNode();
};