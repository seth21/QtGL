#ifndef RESOURCECONFIG_H
#define RESOURCECONFIG_H
#include <vector>
#include <string>

struct ResourceConfig {
public:
	ResourceConfig() {};
	std::vector<std::string> flags;
	bool flagExists(std::string flag) {
		if (std::find(flags.begin(), flags.end(), flag) != flags.end()) return true;
		return false;
	}
};
#endif