#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include <ffi.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "adapter/runner.hpp"

Runner Runner::create()
{
    Runner runner;

    return runner;
}

void *mallocDataPtr(const std::string &type, const std::string &value)
{
    if (type == "char_str")
    {
        char *str = (char *)malloc(sizeof(char) * value.size());
        strcpy(str, value.c_str());
        return static_cast<void *>(str);
    }
    else if (type == "int32")
    {
        int32_t *int_ptr = (int32_t *)malloc(sizeof(int32_t));
        *int_ptr = std::stoi(value);
        return static_cast<void *>(int_ptr);
    }
    else if (type == "int64")
    {
        int64_t *int_ptr = (int64_t *)malloc(sizeof(int64_t));
        *int_ptr = std::stoll(value);
        return static_cast<void *>(int_ptr);
    }
    else
    {
        return nullptr;
    }
}

void Runner::run()
{
    // 加载动态库
    void *handle = dlopen("/usr/lib/x86_64-linux-gnu/libc.so.6", RTLD_NOW);
    if (handle == nullptr)
    {
        std::cerr << "Failed to load libc.so: " << dlerror() << std::endl;
        return;
    }
    void *printf_sym = dlsym(handle, "printf");

    /*
     * 动态测试（存放测试数据）
     */

    this->ffi_types_.push_back(&ffi_type_pointer);
    this->ffi_types_.push_back(&ffi_type_pointer);

    void *format_ptr = mallocDataPtr("char_str", "Hello %s from ffi!\n");
    this->ffi_values_.push_back(&format_ptr);
    void *msg_ptr = mallocDataPtr("char_str", "MyLibCall");
    this->ffi_values_.push_back(&msg_ptr);

    this->ffi_return_type_ = &ffi_type_sint;
    this->return_value_ptr_ = mallocDataPtr("int32", "0");

    /*
     * 开始调用
     */

    if (ffi_prep_cif_var(&this->cif_, FFI_DEFAULT_ABI, 1, 2, this->ffi_return_type_, this->ffi_types_.data()) != FFI_OK)
    {
        std::cerr << "ffi_prep_cif failed" << std::endl;
        dlclose(handle);
        return;
    }

    std::cout << "ffi prep cif success" << std::endl;

    ffi_call(&this->cif_, FFI_FN(printf), this->return_value_ptr_, ffi_values_.data());
    std::cout << "Return value: " << *static_cast<ffi_arg *>(this->return_value_ptr_) << std::endl;

    // release resources
    free(format_ptr);
    free(msg_ptr);
    free(this->return_value_ptr_);
}
