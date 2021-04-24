#ifndef MESHRENDERERCOMP_H
#define MESHRENDERERCOMP_H
#include <memory>
#include "model.h"

struct MeshRendererComp {
	BoundingBox tempAABB;
	std::shared_ptr<Model> model;
};

#endif