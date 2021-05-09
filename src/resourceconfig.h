#ifndef RESOURCECONFIG_H
#define RESOURCECONFIG_H
#include <vector>
#include <string>
#include <algorithm>
#include<set>

struct ResourceConfig {
public:
	ResourceConfig() {};
	std::set<std::string> flags;
	bool flagExists(std::string flag) {
		const bool is_in = flags.find(flag) != flags.end();
		return is_in;
	}

	bool isEqual(const ResourceConfig &c2) {
		if (flags != c2.flags) return false;
		return true;
	}

	void addFlag(std::string flag) {
		flags.insert(flag);
	}
};
#endif