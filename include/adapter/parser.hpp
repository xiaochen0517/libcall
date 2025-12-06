#ifndef LIBCALL_PARSER_HPP
#define LIBCALL_PARSER_HPP

#include <nlohmann/json.hpp>

#include "types/json_type.hpp"

using json = nlohmann::json;

class JsonParser
{
  public:
    JsonParser() {};
    ~JsonParser() {};

    static RootData parse(const std::string &json_path);
};

#endif // LIBCALL_PARSER_HPP
