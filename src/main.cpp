#include <CLI/CLI.hpp>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>

#include "adapter/runner.hpp"

int main(int argc, char *argv[])
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "Hello, World!" << std::endl;

    CLI::App app{"Lib Call: A simple tool to call dynamic libraries using JSON."};

    uint16_t port = 4221;
    size_t threads = 10;
    std::string directory = "";

    // --directory
    app.add_option("-d,--directory", directory, "Directory to serve files from")->default_val(".");

    CLI11_PARSE(app, argc, argv);

    Runner runner = Runner::create();
    runner.run();

    return 0;
}
