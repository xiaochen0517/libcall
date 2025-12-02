#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "adapter/memory_allocator.hpp"

void *MemoryAllocator::mallocate(const TypeInfo &type_info, const BaseTypeData &base_param_data)
{
    nlohmann::json data = base_param_data.data;
    if (data.is_number())
    {
        void *data_ptr = malloc(type_info.type_size);
        if (data.is_number_integer())
        {
            int64_t num = data.get<int64_t>();
            std::memcpy(data_ptr, &num, type_info.type_size);
        }
        else if (data.is_number_unsigned())
        {
            uint64_t num = data.get<uint64_t>();
            std::memcpy(data_ptr, &num, type_info.type_size);
        }
        else if (data.is_number_float())
        {
            double_t num = data.get<double_t>();
            std::memcpy(data_ptr, &num, type_info.type_size);
        }
        return data_ptr;
    }
    else if (data.is_boolean())
    {
        void *data_ptr = malloc(type_info.type_size);
        bool value = data.get<bool>();
        std::memcpy(data_ptr, &value, type_info.type_size);
        return data_ptr;
    }
    else if (data.is_string())
    {
        std::string value = data.get<std::string>();
        char *str = (char *)malloc(type_info.type_size * (value.size() + 1));
        strcpy(str, value.c_str());
        return static_cast<void *>(str);
    }
    else
    {
        return nullptr;
    }
}
void *MemoryAllocator::mallocate(const TypeInfo &type_info, const StructTypeData &struct_param_data)
{
    return nullptr;
}

void *MemoryAllocator::getDoublePointer(void *data_ptr)
{
    void **double_ptr = (void **)malloc(sizeof(void *));
    *double_ptr = data_ptr;
    return static_cast<void *>(double_ptr);
}
