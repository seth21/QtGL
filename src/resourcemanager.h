#pragma once
#include <unordered_map>
#include <memory>
#include "resource.h"
#include "resourceconfig.h"
#include <stdexcept>
class ResourceManager {
public:
    static ResourceManager& getInstance()
    {
        static ResourceManager instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;

    template<typename T>
    std::shared_ptr<T> load(const std::string& path, ResourceConfig config = ResourceConfig()) {
        static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource");

        auto res = resources[path].lock();
        if (!res) {
            // assuming constructor loads resource
            res = std::make_shared<T>(path, config);
            if (!res->loaded()) throw std::runtime_error(std::string("Resource '") + path + "' could not be loaded");
            else resources[path] = res;
        }
       
        auto return_value = std::dynamic_pointer_cast<T>(res);
        if (!return_value) {
            throw std::runtime_error(std::string("Resource '") + path + "' is already loaded as another type");
        }
        return return_value;
    }
private:
    std::unordered_map<std::string, std::weak_ptr<Resource>> resources;
    ResourceManager(){}
};
