#ifndef LIBCALL_TYPE_REGISTRY_HPP
#define LIBCALL_TYPE_REGISTRY_HPP

#pragma once
#include <ffi.h>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "adapter/libcall_type.hpp"
#include "types/json_type.hpp"

/**
 * 类型注册生成器
 */
class FFITypeRegistry
{
  public:
    static FFITypeRegistry &getInstance()
    {
        static FFITypeRegistry instance;
        return instance;
    }

    FFITypeRegistry(const FFITypeRegistry &) = delete;
    FFITypeRegistry &operator=(const FFITypeRegistry &) = delete;

    static LCBaseTypeInfo parse(BaseTypeData &base_param_data);
    static LCStructTypeInfo parse(StructTypeData &struct_param_data);
    static LCLibInfo parse(LibDataInfo &lib_data_info);
    static LCFuncCallInfo parse(FuncCallDataInfo &func_call_data_info);

    void registerData(std::vector<LCBaseTypeInfo> lc_type_list);
    void registerData(std::vector<LCStructTypeInfo> lc_struct_type_list);
    void registerData(std::vector<LCLibInfo> lc_lib_list);
    void registerData(std::vector<LCFuncCallInfo> lc_func_call_list);

    LCBaseTypeInfo getBaseTypeInfo(const std::string &label_name)
    {
        if (this->base_type_info_map_.find(label_name) == this->base_type_info_map_.end())
        {
            throw std::runtime_error("Base label name not found: " + label_name);
        }
        return this->base_type_info_map_.at(label_name);
    }
    LCStructTypeInfo getStructTypeInfo(const std::string &label_name)
    {
        if (this->struct_type_info_map_.find(label_name) == this->struct_type_info_map_.end())
        {
            throw std::runtime_error("Struct label name not found: " + label_name);
        }
        return this->struct_type_info_map_.at(label_name);
    }
    LCLibInfo getLibInfo(const std::string &label_name)
    {
        if (this->lib_info_map_.find(label_name) == this->lib_info_map_.end())
        {
            throw std::runtime_error("Lib label name not found: " + label_name);
        }
        return this->lib_info_map_.at(label_name);
    }
    LCFuncCallInfo getFuncCallInfo(const std::string &label_name)
    {
        if (this->func_call_info_map_.find(label_name) == this->func_call_info_map_.end())
        {
            throw std::runtime_error("Func call label name not found: " + label_name);
        }
        return this->func_call_info_map_.at(label_name);
    }
    std::vector<LCFuncCallInfo> getLCFuncCallInfoList()
    {
        std::vector<LCFuncCallInfo> func_call_info_list;
        for (const auto &pair : this->func_call_info_map_)
        {
            func_call_info_list.push_back(pair.second);
        }
        return func_call_info_list;
    }

    void clean();

  private:
    FFITypeRegistry() {};

    std::unordered_map<std::string, LCBaseTypeInfo> base_type_info_map_;
    std::unordered_map<std::string, LCStructTypeInfo> struct_type_info_map_;
    std::unordered_map<std::string, LCLibInfo> lib_info_map_;
    std::unordered_map<std::string, LCFuncCallInfo> func_call_info_map_;
};

extern FFITypeRegistry &TYPE_REGISTRY;

#endif // LIBCALL_TYPE_REGISTRY_HPP
