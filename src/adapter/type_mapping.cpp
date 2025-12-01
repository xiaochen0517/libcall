#include <stdexcept>

#include "adapter/type_mapping.hpp"

TypeInfo TypeMapping::getTypeInfo(const std::string &type_name)
{
    auto it = TYPE_MAP.find(type_name);
    if (it != TYPE_MAP.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Unsupported type: " + type_name);
    }
}
