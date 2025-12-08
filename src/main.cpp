#include <CLI/CLI.hpp>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>
#include <string>

#include "adapter/runner.hpp"
#include "conf.hpp"

int main(int argc, char *argv[])
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << std::endl;
    std::cout << "========" << std::endl;
    std::cout << "Project name: " << PROJECT_NAME << std::endl;
    std::cout << "Project description: " << PROJECT_DESCRIPTION << std::endl;
    std::cout << "========" << std::endl;
    std::cout << std::endl;

    CLI::App app{PROJECT_DESCRIPTION};

    uint16_t port = 4221;
    size_t threads = 10;
    std::string json_path = "";

    // --directory
    app.add_option("-j,--json", json_path, "Json config file path")->default_val("");

    CLI11_PARSE(app, argc, argv);

    RunParam param;
    param.json_path = json_path;

    Runner runner = Runner::create();
    runner.run(param);

    return 0;
}
