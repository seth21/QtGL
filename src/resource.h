#pragma once
#include <string>
class Resource {
public:
    Resource(const std::string &file) {}
    virtual ~Resource() {}
    virtual bool loaded() { return false; }
};