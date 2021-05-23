#ifndef HIERARCHYCOMP_H
#define HIERARCHYCOMP_H
#include <memory>
class entity::entt;

struct HierarchyComp {
public:
	HierarchyComp() {

	}
	entt::entity parent{ entt::null };
	unsigned char hierarchyDepth = 0;

	std::vector<entt::entity>& getChildren() {
		return children;
	}
private:
	std::vector<entt::entity> children;
};

#endif