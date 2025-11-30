#include <fstream>
#include <iostream>

#include "adapter/parser.hpp"

void parser_func()
{
    std::ifstream file("test.json");
    json data = json::parse(file);
    std::string name = data["name"];
    std::cout << "Name: " << name << std::endl;
}
