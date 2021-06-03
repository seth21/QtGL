#ifndef HIERARCHYCOMP_H
#define HIERARCHYCOMP_H
#include <memory>
//class entt::entity;

struct HierarchyComp {
public:
	HierarchyComp(entt::entity par) {
		parent = par;
		oldParent = par;
	}
	HierarchyComp(entt::entity par, unsigned char depth) {
		parent = par;
		oldParent = par;
		hierarchyDepth = depth;
	}
	
	unsigned char hierarchyDepth = 0;

	std::vector<entt::entity>& getChildren() {
		return children;
	}

	void setParent(entt::entity par) {
		oldParent = parent;
		parent = par;
	}

	entt::entity getParent() {
		return parent;
	}

	bool parentChanged() {
		return oldParent != parent;
	}

	entt::entity getPreviousParent() {
		return oldParent;
	}

	void invalidateParentChange() {
		oldParent = parent;
	}
private:
	std::vector<entt::entity> children;
	entt::entity parent{ entt::null };
	entt::entity oldParent{ entt::null };
};

#endif