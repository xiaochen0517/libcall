#ifndef LIBCALL_PARSER_HPP
#define LIBCALL_PARSER_HPP

#include <nlohmann/json.hpp>

using json = nlohmann::json;

void parser_func();

class Parser
{
  public:
    Parser() {};
    ~Parser() {};
};

#endif // LIBCALL_PARSER_HPP
