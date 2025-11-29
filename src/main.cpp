#include <CLI/CLI.hpp>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>

void run()
{
    // 加载动态库
    void *handle = dlopen("/usr/lib/x86_64-linux-gnu/libc.so.6", RTLD_NOW);
    if (handle == nullptr)
    {
        std::cerr << "Failed to load libc.so: " << dlerror() << std::endl;
        return;
    }
    std::cout << "Load lib address: " << handle << std::endl;
    // 加载动态库中的函数符号
    typedef int (*printf_t)(const char *, ...);
    printf_t my_printf = (printf_t)dlsym(handle, "printf");
    if (!my_printf)
    {
        std::cerr << "Failed to find printf symbol: " << dlerror() << std::endl;
        dlclose(handle);
        return;
    }
    // 调用函数
    my_printf("Hello from dynamically loaded printf!\n");
}

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

    run();

    return 0;
}
