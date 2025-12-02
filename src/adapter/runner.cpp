#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include <ffi.h>
#include <format>
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

    std::vector<LCBaseTypeInfo> lc_base_type_info_list;
    for (BaseTypeData base_type_data : root_data.base_types)
    {
        LCBaseTypeInfo lc_type_info = FFITypeRegistry::parse(base_type_data);
        std::cout << "Base label: " << lc_type_info.getLabelName() << ", type: " << lc_type_info.getTypeName()
                  << std::endl;
        lc_base_type_info_list.push_back(lc_type_info);
    }
    type_registry.registerData(lc_base_type_info_list);

    std::vector<LCLibInfo> lc_lib_info_list;
    for (LibDataInfo &lib_data_info : root_data.libs)
    {
        LCLibInfo lib_info = FFITypeRegistry::parse(lib_data_info);
        std::cout << "Lib label: " << lib_info.getLabelName() << ", path: " << lib_info.getLibPath()
                  << ", handle: " << lib_info.getLibHandle() << std::endl;
        lc_lib_info_list.push_back(lib_info);
    }
    type_registry.registerData(lc_lib_info_list);

    std::vector<LCFuncCallInfo> lc_func_call_info_list;
    for (FuncCallDataInfo &func_call_data_info : root_data.func_calls)
    {
        LCFuncCallInfo func_call_info = FFITypeRegistry::parse(func_call_data_info);
        std::cout << "Func call label: " << func_call_info.getLabelName()
                  << ", lib label: " << func_call_info.getLibLabel() << ", func name: " << func_call_info.getFuncName()
                  << std::endl;
        lc_func_call_info_list.push_back(func_call_info);
    }
    type_registry.registerData(lc_func_call_info_list);

    /*
     * 动态测试（存放测试数据）
     */

    for (LCFuncCallInfo func_call_info : lc_func_call_info_list)
    {
        std::cout << "Function Call Info: " << func_call_info.getFuncName() << std::endl;
        LCLibInfo glibc_info = type_registry.getLibInfo(func_call_info.getLibLabel());
        void *printf_sym = dlsym(glibc_info.getLibHandle(), func_call_info.getFuncName().c_str());
        if (printf_sym == nullptr)
        {
            throw std::runtime_error(
                std::format("Execute function: {} error because failed to get symbol: {} from library: {}",
                            func_call_info.getLabelName(), func_call_info.getFuncName(), glibc_info.getLibPath()));
        }
        for (const std::string &param_label : func_call_info.getParamLabels())
        {
            LCBaseTypeInfo lc_type_info = type_registry.getBaseTypeInfo(param_label);
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

        LCBaseTypeInfo result_base_type = type_registry.getBaseTypeInfo(func_call_info.getReturnLabel());
        this->ffi_return_type_ = result_base_type.getFFITypePtr();
        this->return_value_ptr_ = result_base_type.getDataPtr();

        /*
         * 开始调用
         */

        if (ffi_prep_cif_var(&this->cif_, FFI_DEFAULT_ABI, 1, 2, this->ffi_return_type_, this->ffi_types_.data()) !=
            FFI_OK)
        {
            throw std::runtime_error("ffi_prep_cif failed");
        }

        std::cout << "ffi prep cif success" << std::endl;

        ffi_call(&this->cif_, FFI_FN(printf), this->return_value_ptr_, ffi_values_.data());
        std::cout << "Return value: " << *static_cast<ffi_arg *>(this->return_value_ptr_) << std::endl;
    }
    // release resources
    free(this->return_value_ptr_);
}
