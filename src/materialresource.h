#ifndef MATERIALRESOURCE_H
#define MATERIALRESOURCE_H
#include <string>

class Material;

struct MaterialResource {
	Material* material;
	int references;
	std::string resourcePath;
	int materialID;
};

#endif