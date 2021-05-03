#pragma once
#include <string>
#include "resourceconfig.h"
class Resource {
public:
    Resource(const std::string &file, ResourceConfig config) {}
    virtual ~Resource() {}
    virtual bool loaded() { return false; }
    virtual ResourceConfig& getResourceConfig() = 0;
};