#include <iostream>
#include <stdexcept>

#include "adapter/memory_allocator.hpp"
#include "adapter/type_mapping.hpp"
#include "adapter/type_registry.hpp"

LCTypeInfo FFITypeRegistry::parse(BaseParamData &base_param_data)
{
    TypeInfo type_info = TypeMapping::getTypeInfo(base_param_data.type_name);
    void *data_ptr = MemoryAllocator::mallocate(type_info, base_param_data);
    void *double_data_ptr = MemoryAllocator::getDoublePointer(data_ptr);
    return LCTypeInfo(base_param_data.type_name, base_param_data.label_name, type_info.ffi_type_ptr, data_ptr, double_data_ptr);
}
LCStructTypeInfo FFITypeRegistry::parse(StructParamData &struct_param_data)
{
    TypeInfo type_info = TypeMapping::getTypeInfo("pointer");
    return LCStructTypeInfo();
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
void FFITypeRegistry::registerData(std::vector<LCStructTypeInfo> &lc_struct_type_list)
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
