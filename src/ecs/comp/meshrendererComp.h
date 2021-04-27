#ifndef MESHRENDERERCOMP_H
#define MESHRENDERERCOMP_H
#include <memory>
#include "model.h"

struct MeshRendererComp {
	BoundingBox transformedAABB;
	std::shared_ptr<Model> m_model;
	int m_meshIndex = -1;
	MeshRendererComp(std::shared_ptr<Model> model, int meshIndex) {
		m_meshIndex = meshIndex;
		m_model = model;
	}
	MeshRendererComp(std::shared_ptr<Model> model) {
		m_meshIndex = -1;
		m_model = model;
	}
};

#endif