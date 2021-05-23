#ifndef MESHRENDERERCOMP_H
#define MESHRENDERERCOMP_H
#include <memory>
#include "model.h"
#include "boundingsphere.h"

struct MeshRendererComp {
	//BoundingBox transformedAABB;
	//BoundingSphere boundingSphere;
	std::shared_ptr<Model> m_model;
	
	std::vector<unsigned int> meshIndices;
	std::string m_nodeName;

	MeshRendererComp(std::shared_ptr<Model> model, std::vector<unsigned int>& meshInds, std::string nodeName) {
		m_model = model;
		m_nodeName = nodeName;
		for (auto idx : meshInds) {
			meshIndices.push_back(idx);
		}
	}
	MeshRendererComp(std::shared_ptr<Model> model, unsigned int meshIndex, std::string nodeName) {
		m_model = model;
		m_nodeName = nodeName;
		meshIndices.push_back(meshIndex);
	}
};

#endif