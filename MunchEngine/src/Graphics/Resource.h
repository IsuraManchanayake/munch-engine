#pragma once

#include <utility>

struct Resource {
    Resource()
        : copied(false) {
    }
    
    Resource(const Resource& resource) 
        : copied(resource.copied) {
        resource.copied = true;
    }

    Resource& operator=(const Resource& resource) {
        clear();
        copied = resource.copied;
        resource.copied = true;
        return *this;
    }

    Resource(Resource&& resource) noexcept
        : copied(resource.copied) {
    }

    Resource& operator=(Resource&& resource) noexcept {
        clear();
        copied = std::exchange(resource.copied, false);
        resource.copied = true;
        return *this;
    }

    virtual void clear() = 0;
    
    mutable bool copied;
};

#define ADD_SPECIAL_CLASS_FUNCTIONS(ClassName)          \
    ClassName(const ClassName&) = default;              \
    ClassName& operator=(const ClassName&) = default;   \
    ClassName(ClassName&&) = default;                   \
    ClassName& operator=(ClassName&&) = default;        
