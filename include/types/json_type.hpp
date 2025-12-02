#ifndef LIBCALL_JSON_TYPE_HPP
#define LIBCALL_JSON_TYPE_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

// 构造体数据信息
struct StructDataInfo
{
    std::string type_name;
    nlohmann::json data;
};

// 结构体参数数据
struct StructTypeData
{
    std::string type_name;
    std::vector<StructDataInfo> struct_data_list;
};

// 基础数据类型参数数据
struct BaseTypeData
{
    std::string label_name;
    std::string type_name;
    nlohmann::json data;
};

// 动态库信息
struct LibDataInfo
{
    std::string label_name;
    std::string lib_path;
};

// 函数调用信息
struct FuncCallDataInfo
{
    std::string label_name;
    std::string lib_label;
    std::string func_name;
    std::vector<std::string> param_labels;
    std::string return_label;
};

struct RootData
{
    std::vector<BaseTypeData> base_types;
    std::vector<StructTypeData> struct_types;
    std::vector<LibDataInfo> libs;
    std::vector<FuncCallDataInfo> func_calls;
};

#endif // LIBCALL_JSON_TYPE_HPP
