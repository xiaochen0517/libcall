#ifndef LIBCALL_TYPE_REGISTRY_HPP
#define LIBCALL_TYPE_REGISTRY_HPP

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
    FFITypeRegistry() {};
    ~FFITypeRegistry() {};

    static LCTypeInfo parse(BaseParamData &base_param_data);
    static LCStructTypeInfo parse(StructParamData &struct_param_data);

    void registerData(std::vector<LCTypeInfo> lc_type_list);
    void registerData(std::vector<LCStructTypeInfo> &lc_struct_type_list);

    LCTypeInfo getBaseTypeInfo(const std::string &label_name)
    {
        return this->base_type_info_map_.at(label_name);
    }
    LCStructTypeInfo getStructTypeInfo(const std::string &label_name)
    {
        return this->struct_type_info_map_.at(label_name);
    }

  private:
    std::unordered_map<std::string, LCTypeInfo> base_type_info_map_;
    std::unordered_map<std::string, LCStructTypeInfo> struct_type_info_map_;
};

#endif // LIBCALL_TYPE_REGISTRY_HPP
