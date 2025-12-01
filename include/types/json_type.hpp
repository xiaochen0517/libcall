#ifndef LIBCALL_JSON_TYPE_HPP
#define LIBCALL_JSON_TYPE_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

// 构造体数据信息
struct StructDataInfo
{
    std::string type;
    nlohmann::json data;
};

// 结构体参数数据
struct StructParamData
{
    std::string type_name;
    std::vector<StructDataInfo> struct_data_list;
};

// 基础数据类型参数数据
struct BaseParamData
{
    std::string label_name;
    std::string type_name;
    nlohmann::json data;
};

#endif // LIBCALL_JSON_TYPE_HPP
