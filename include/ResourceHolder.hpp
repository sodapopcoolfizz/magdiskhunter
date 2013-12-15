#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>


template <typename Resource, typename Identifier>
class ResourceHolder
{
    public:
    void load(Identifier id, const std::string& filename);
    template <typename Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam);
    ~ResourceHolder();

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;

    private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};
#include "../src/ResourceHolder.inl"
#endif //RESOURCEHOLDER_HPP
