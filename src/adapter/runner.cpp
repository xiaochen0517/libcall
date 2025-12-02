#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include <ffi.h>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "adapter/libcall_type.hpp"
#include "adapter/parser.hpp"
#include "adapter/runner.hpp"
#include "adapter/type_registry.hpp"
#include "types/json_type.hpp"

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

    RootData root_data = JsonParser::parse();

    FFITypeRegistry type_registry;

    BaseTypeData formatParamData = BaseTypeData{
        .label_name = "format",
        .type_name = "cstring",
        .data = nlohmann::json("Hello %s from ffi!\n"),
    };
    BaseTypeData msgParamData = BaseTypeData{
        .label_name = "msg",
        .type_name = "cstring",
        .data = nlohmann::json("MyLibCall"),
    };

    LCTypeInfo formatTypeInfo = FFITypeRegistry::parse(formatParamData);
    LCTypeInfo msgTypeInfo = FFITypeRegistry::parse(msgParamData);

    type_registry.registerData(std::vector<LCTypeInfo>{formatTypeInfo, msgTypeInfo});

    std::vector<LCLibInfo> lc_lib_info_list;
    for (LibDataInfo &lib_data_info : root_data.libs)
    {
        LCLibInfo lib_info = FFITypeRegistry::parse(lib_data_info);
        std::cout << "Lib label: " << lib_info.getLabelName() << ", path: " << lib_info.getLibPath()
                  << ", handle: " << lib_info.getLibHandle() << std::endl;
        lc_lib_info_list.push_back(lib_info);
    }
    type_registry.registerData(lc_lib_info_list);

    // 加载动态库
    // void *handle = dlopen("/usr/lib/x86_64-linux-gnu/libc.so.6", RTLD_NOW);
    // if (handle == nullptr)
    // {
    //     std::cerr << "Failed to load libc.so: " << dlerror() << std::endl;
    //     return;
    // }
    LCLibInfo glibc_info = type_registry.getLibInfo("glibc");
    std::cout << "Lib Path: " << glibc_info.getLibPath() << std::endl;
    std::cout << "Lib Handle: " << glibc_info.getLibHandle() << std::endl;

    void *printf_sym = dlsym(glibc_info.getLibHandle(), "printf");

    /*
     * 动态测试（存放测试数据）
     */

    std::vector<std::string> param_labels = {"format", "msg"};

    for (const std::string &param_label : param_labels)
    {
        LCTypeInfo lc_type_info = type_registry.getBaseTypeInfo(param_label);
        this->ffi_types_.push_back(lc_type_info.getFFITypePtr());
        this->ffi_values_.push_back(lc_type_info.getDoubleDataPtr());
    }

    std::cout << "FFI Types Size: " << this->ffi_types_.size() << std::endl;
    std::cout << "FFI Values Size: " << this->ffi_values_.size() << std::endl;

    for (void *ffi_value_ptr : this->ffi_values_)
    {
        std::cout << "FFI Value Ptr: " << ffi_value_ptr << std::endl;
        printf("FFI Value Content: %s\n", *(char **)ffi_value_ptr);
    }

    // this->ffi_types_.push_back(&ffi_type_pointer);
    // this->ffi_types_.push_back(&ffi_type_pointer);
    //
    // void *format_ptr = mallocDataPtr("char_str", "Hello %s from ffi!\n");
    // this->ffi_values_.push_back(&format_ptr);
    // void *msg_ptr = mallocDataPtr("char_str", "MyLibCall");
    // this->ffi_values_.push_back(&msg_ptr);

    this->ffi_return_type_ = &ffi_type_sint;
    this->return_value_ptr_ = mallocDataPtr("int32", "0");

    /*
     * 开始调用
     */

    if (ffi_prep_cif_var(&this->cif_, FFI_DEFAULT_ABI, 1, 2, this->ffi_return_type_, this->ffi_types_.data()) != FFI_OK)
    {
        throw std::runtime_error("ffi_prep_cif failed");
    }

    std::cout << "ffi prep cif success" << std::endl;

    ffi_call(&this->cif_, FFI_FN(printf), this->return_value_ptr_, ffi_values_.data());
    std::cout << "Return value: " << *static_cast<ffi_arg *>(this->return_value_ptr_) << std::endl;

    // release resources
    free(this->return_value_ptr_);
}
