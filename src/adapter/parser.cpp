#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "adapter/parser.hpp"
#include "types/json_type.hpp"

BaseTypeData parseBaseTypeData(const json &data)
{
    BaseTypeData base_type_data;
    base_type_data.label_name = data.at("label_name").get<std::string>();
    if (base_type_data.label_name.empty())
    {
        std::cerr << "label_name is empty" << std::endl;
    }
    base_type_data.type_name = data.at("type_name").get<std::string>();
    if (base_type_data.type_name.empty())
    {
        std::cerr << "type_name is empty" << std::endl;
    }
    base_type_data.data = data.at("data");
    return base_type_data;
}

std::vector<BaseTypeData> parseBaseTypeDataList(std::set<std::string> &labels, const json root_json)
{
    // 检查 base_types 是否存在，且类型是否为数组
    json::const_iterator it = root_json.find("base_types");
    if (it == root_json.end() || !it->is_array())
    {
        std::cerr << "base_types not found or is not an array" << std::endl;
        return {};
    }
    // 开始解析 base_types
    std::vector<BaseTypeData> base_type_data_list;
    for (const auto &item : root_json.at("base_types"))
    {
        BaseTypeData base_type_data = parseBaseTypeData(item);
        if (labels.find(base_type_data.label_name) != labels.end())
        {
            std::cerr << "Duplicate label name found: " << base_type_data.label_name << std::endl;
            continue;
        }
        labels.insert(base_type_data.label_name);
        base_type_data_list.push_back(base_type_data);
    }
    return base_type_data_list;
}

LibDataInfo parseLibDataInfo(const json &data)
{
    LibDataInfo lib_data_info;
    lib_data_info.label_name = data.at("label_name").get<std::string>();
    lib_data_info.lib_path = data.at("lib_path").get<std::string>();
    return lib_data_info;
}

std::vector<LibDataInfo> parseLibDataInfoList(std::set<std::string> &labels, const json root_json)
{
    // 检查 libs 是否存在，且类型是否为数组
    json::const_iterator it = root_json.find("libs");
    if (it == root_json.end() || !it->is_array())
    {
        std::cerr << "libs not found or is not an array" << std::endl;
        return {};
    }
    // 开始解析 libs
    std::vector<LibDataInfo> lib_data_info_list;
    for (const auto &item : root_json.at("libs"))
    {
        LibDataInfo lib_data_info = parseLibDataInfo(item);
        if (labels.find(lib_data_info.label_name) != labels.end())
        {
            std::cerr << "Duplicate label name found: " << lib_data_info.label_name << std::endl;
            continue;
        }
        labels.insert(lib_data_info.label_name);
        lib_data_info_list.push_back(lib_data_info);
    }
    return lib_data_info_list;
}

FuncCallDataInfo parseFuncCallDataInfo(const json &data)
{
    FuncCallDataInfo func_call_data_info;
    func_call_data_info.label_name = data.at("label_name").get<std::string>();
    func_call_data_info.lib_label = data.at("lib_label").get<std::string>();
    func_call_data_info.func_name = data.at("func_name").get<std::string>();
    func_call_data_info.param_labels = data.at("param_labels").get<std::vector<std::string>>();
    func_call_data_info.return_label = data.at("return_label").get<std::string>();
    return func_call_data_info;
}

std::vector<FuncCallDataInfo> parseFuncCallDataInfoList(std::set<std::string> &labels, const json root_json)
{
    // 检查 func_calls 是否存在，且类型是否为数组
    json::const_iterator it = root_json.find("func_calls");
    if (it == root_json.end() || !it->is_array())
    {
        std::cerr << "func_calls not found or is not an array" << std::endl;
        return {};
    }
    // 开始解析 func_calls
    std::vector<FuncCallDataInfo> func_call_data_info_list;
    for (const auto &item : root_json.at("func_calls"))
    {
        FuncCallDataInfo func_call_data_info = parseFuncCallDataInfo(item);
        func_call_data_info_list.push_back(func_call_data_info);
    }
    return func_call_data_info_list;
}

RootData JsonParser::parse()
{
    std::ifstream file("test.json");
    json data = json::parse(file);

    std::set<std::string> labels;

    RootData root_data;
    root_data.base_types = parseBaseTypeDataList(labels, data);
    // TODO 构造体类型解析
    // TODO 回调函数解析
    root_data.libs = parseLibDataInfoList(labels, data);
    root_data.func_calls = parseFuncCallDataInfoList(labels, data);
    return root_data;
}
