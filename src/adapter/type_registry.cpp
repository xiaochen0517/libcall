#include <dlfcn.h>
#include <iostream>
#include <stdexcept>

#include "adapter/memory_allocator.hpp"
#include "adapter/type_mapping.hpp"
#include "adapter/type_registry.hpp"

LCTypeInfo FFITypeRegistry::parse(BaseTypeData &base_param_data)
{
    TypeInfo type_info = TypeMapping::getTypeInfo(base_param_data.type_name);
    void *data_ptr = MemoryAllocator::mallocate(type_info, base_param_data);
    void *double_data_ptr = MemoryAllocator::getDoublePointer(data_ptr);
    return LCTypeInfo(base_param_data.type_name, base_param_data.label_name, type_info.ffi_type_ptr, data_ptr,
                      double_data_ptr);
}
LCStructTypeInfo FFITypeRegistry::parse(StructTypeData &struct_param_data)
{
    TypeInfo type_info = TypeMapping::getTypeInfo("pointer");
    // TODO: 完善结构体类型的解析
    return LCStructTypeInfo();
}
LCLibInfo FFITypeRegistry::parse(LibDataInfo &lib_data_info)
{
    void *handle = dlopen(lib_data_info.lib_path.c_str(), RTLD_NOW);
    if (handle == nullptr)
    {
        throw std::runtime_error("Failed to load library: " + lib_data_info.lib_path + " Error: " + dlerror());
    }
    return LCLibInfo(lib_data_info.label_name, lib_data_info.lib_path, handle);
}

void FFITypeRegistry::registerData(std::vector<LCTypeInfo> lc_type_list)
{
    for (const LCTypeInfo &lc_type_info : lc_type_list)
    {
        if (this->base_type_info_map_.find(lc_type_info.getLabelName()) != this->base_type_info_map_.end())
        {
            throw std::runtime_error("Duplicate base label name registration: " + lc_type_info.getLabelName());
        }
        std::cout << "Add Base Type Info: " << lc_type_info.getLabelName() << " -> " << lc_type_info.getTypeName()
                  << std::endl;
        this->base_type_info_map_.insert({lc_type_info.getLabelName(), lc_type_info});
    }
    std::cout << "Base Type Info Map Size: " << this->base_type_info_map_.size() << std::endl;
}
void FFITypeRegistry::registerData(std::vector<LCStructTypeInfo> lc_struct_type_list)
{
    for (const LCStructTypeInfo &lc_struct_type_info : lc_struct_type_list)
    {
        if (this->struct_type_info_map_.find(lc_struct_type_info.getLabelName()) != this->struct_type_info_map_.end())
        {
            throw std::runtime_error("Duplicate struct label name registration: " + lc_struct_type_info.getLabelName());
        }
        this->struct_type_info_map_.insert({lc_struct_type_info.getLabelName(), lc_struct_type_info});
    }
}
void FFITypeRegistry::registerData(std::vector<LCLibInfo> lc_lib_list)
{
    for (const LCLibInfo &lc_lib_info : lc_lib_list)
    {
        if (this->lib_info_map_.find(lc_lib_info.getLabelName()) != this->lib_info_map_.end())
        {
            throw std::runtime_error("Duplicate lib label name registration: " + lc_lib_info.getLabelName());
        }
        this->lib_info_map_.insert({lc_lib_info.getLabelName(), lc_lib_info});
    }
}
