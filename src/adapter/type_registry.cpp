#include <dlfcn.h>
#include <format>
#include <stdexcept>

#include "adapter/memory_allocator.hpp"
#include "adapter/type_mapping.hpp"
#include "adapter/type_registry.hpp"
#include "logger/logger.hpp"

FFITypeRegistry &TYPE_REGISTRY = FFITypeRegistry::getInstance();

LCBaseTypeInfo FFITypeRegistry::parse(BaseTypeData &base_param_data)
{
    TypeInfo type_info = TypeMapping::getTypeInfo(base_param_data.data_type);
    void *data_ptr = MemoryAllocator::mallocate(type_info, base_param_data);
    void *double_data_ptr = MemoryAllocator::getDoublePointer(data_ptr);
    return LCBaseTypeInfo(base_param_data.label_name, base_param_data.type, base_param_data.data_type,
                          type_info.ffi_type_ptr, data_ptr, double_data_ptr);
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
LCFuncCallInfo FFITypeRegistry::parse(FuncCallDataInfo &func_call_data_info)
{
    return LCFuncCallInfo(func_call_data_info.label_name, func_call_data_info.lib_label, func_call_data_info.func_name,
                          func_call_data_info.variadic, func_call_data_info.fixed_param_count,
                          func_call_data_info.param_labels, func_call_data_info.return_label);
}

void FFITypeRegistry::registerData(std::vector<LCBaseTypeInfo> lc_type_list)
{
    for (const LCBaseTypeInfo &lc_type_info : lc_type_list)
    {
        if (this->base_type_info_map_.find(lc_type_info.getLabelName()) != this->base_type_info_map_.end())
        {
            throw std::runtime_error("Duplicate base label name registration: " + lc_type_info.getLabelName());
        }
        LOGGER.info(std::format("Add Base Type: {}->{};", lc_type_info.getLabelName(), lc_type_info.getDataType()));
        this->base_type_info_map_.insert({lc_type_info.getLabelName(), lc_type_info});
    }
    LOGGER.info(std::format("Base Type Info Map Size: {}", this->base_type_info_map_.size()));
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
        LOGGER.info(
            std::format("Registered library: {} at path: {}", lc_lib_info.getLabelName(), lc_lib_info.getLibPath()));
    }
}
void FFITypeRegistry::registerData(std::vector<LCFuncCallInfo> lc_func_call_list)
{
    for (const LCFuncCallInfo &lc_func_call_info : lc_func_call_list)
    {
        if (this->func_call_info_map_.find(lc_func_call_info.getFuncName()) != this->func_call_info_map_.end())
        {
            throw std::runtime_error("Duplicate func call label name registration: " + lc_func_call_info.getFuncName());
        }
        this->func_call_info_map_.insert({lc_func_call_info.getFuncName(), lc_func_call_info});
        LOGGER.info(std::format("Registered function call: {} for library label: {}", lc_func_call_info.getFuncName(),
                                lc_func_call_info.getLibLabel()));
    }
}

void FFITypeRegistry::clean()
{
    if (!this->lib_info_map_.empty())
    {
        for (auto &pair : this->lib_info_map_)
        {
            LCLibInfo &lib_info = pair.second;
            if (lib_info.getLibHandle() != nullptr)
            {
                dlclose(lib_info.getLibHandle());
            }
        }
        this->lib_info_map_.clear();
    }
    if (!this->base_type_info_map_.empty())
    {
        for (auto &pair : this->base_type_info_map_)
        {
            LCBaseTypeInfo &base_type_info = pair.second;
            MemoryAllocator::deallocate(base_type_info.getDoubleDataPtr());
            MemoryAllocator::deallocate(base_type_info.getDataPtr());
        }
        this->base_type_info_map_.clear();
    }
    // if (!this->struct_type_info_map_.empty())
    // {
    //     for (auto &pair : this->struct_type_info_map_)
    //     {
    //         LCStructTypeInfo &struct_type_info = pair.second;
    //         MemoryAllocator::deallocate(struct_type_info.getDataPtr());
    //     }
    //     this->struct_type_info_map_.clear();
    // }
    if (!this->func_call_info_map_.empty())
    {
        this->func_call_info_map_.clear();
    }
}
