#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include <ffi.h>
#include <format>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "adapter/libcall_type.hpp"
#include "adapter/parser.hpp"
#include "adapter/runner.hpp"
#include "adapter/type_registry.hpp"
#include "logger/logger.hpp"
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

void parseJsonAndRegisterTypes(const std::string &json_path)
{
    LOGGER.info(std::format("Current json config path: {}", json_path));

    RootData root_data = JsonParser::parse(json_path);

    std::vector<LCBaseTypeInfo> lc_base_type_info_list;
    for (BaseTypeData base_type_data : root_data.base_types)
    {
        LCBaseTypeInfo lc_type_info = FFITypeRegistry::parse(base_type_data);
        lc_base_type_info_list.push_back(lc_type_info);
    }
    TYPE_REGISTRY.registerData(lc_base_type_info_list);

    std::vector<LCLibInfo> lc_lib_info_list;
    for (LibDataInfo &lib_data_info : root_data.libs)
    {
        LCLibInfo lib_info = FFITypeRegistry::parse(lib_data_info);
        lc_lib_info_list.push_back(lib_info);
    }
    TYPE_REGISTRY.registerData(lc_lib_info_list);

    std::vector<LCFuncCallInfo> lc_func_call_info_list;
    for (FuncCallDataInfo &func_call_data_info : root_data.func_calls)
    {
        LCFuncCallInfo func_call_info = FFITypeRegistry::parse(func_call_data_info);
        lc_func_call_info_list.push_back(func_call_info);
    }
    TYPE_REGISTRY.registerData(lc_func_call_info_list);
}

void Runner::run(RunParam &param)
{
    parseJsonAndRegisterTypes(param.json_path);

    /*
     * 动态测试（存放测试数据）
     */
    std::cout << std::endl;
    LOGGER.info("Start to execute function calls...");

    auto lc_func_call_info_list = TYPE_REGISTRY.getLCFuncCallInfoList();
    for (LCFuncCallInfo func_call_info : lc_func_call_info_list)
    {
        LOGGER.info(std::format("Execute function call, label: {}; symbol: {};", func_call_info.getLabelName(),
                                func_call_info.getFuncName()));

        LCLibInfo glibc_info = TYPE_REGISTRY.getLibInfo(func_call_info.getLibLabel());
        void *function_sym = dlsym(glibc_info.getLibHandle(), func_call_info.getFuncName().c_str());
        if (function_sym == nullptr)
        {
            throw std::runtime_error(
                std::format("Execute function: {} error because failed to get symbol: {} from library: {}",
                            func_call_info.getLabelName(), func_call_info.getFuncName(), glibc_info.getLibPath()));
        }
        for (const std::string &param_label : func_call_info.getParamLabels())
        {
            LCBaseTypeInfo lc_type_info = TYPE_REGISTRY.getBaseTypeInfo(param_label);
            if (lc_type_info.getType() == "pointer")
            {
                this->ffi_types_.push_back(&ffi_type_pointer);
                this->ffi_values_.push_back(lc_type_info.getDoubleDataPtr());
            }
            else if (lc_type_info.getType() == "literal")
            {
                this->ffi_types_.push_back(lc_type_info.getFFITypePtr());
                this->ffi_values_.push_back(lc_type_info.getDataPtr());
            }
            else
            {
                throw std::runtime_error("Unsupported type: " + lc_type_info.getType());
            }
        }

        LOGGER.info(std::format("FFI Types Size: {}", this->ffi_types_.size()));
        LOGGER.info(std::format("FFI Values Size: {}", this->ffi_values_.size()));

        LOGGER.info("Preparing to call function via FFI...");
        for (void *ffi_value_ptr : this->ffi_values_)
        {
            LOGGER.info(std::format("FFI Value Ptr: 0x{:x}", (uintptr_t)ffi_value_ptr));
        }

        LCBaseTypeInfo result_base_type = TYPE_REGISTRY.getBaseTypeInfo(func_call_info.getReturnLabel());
        LOGGER.info(std::format("Return Type Label: {}", result_base_type.getLabelName()));
        if (result_base_type.getType() == "literal")
        {
            this->ffi_return_type_ = result_base_type.getFFITypePtr();
            this->return_value_ptr_ = result_base_type.getDataPtr();
        }
        else if (result_base_type.getType() == "pointer")
        {
            this->ffi_return_type_ = &ffi_type_pointer;
            this->return_value_ptr_ = result_base_type.getDoubleDataPtr();
        }
        else
        {
            throw std::runtime_error("Unsupported return type: " + result_base_type.getType());
        }
        LOGGER.info(std::format("FFI Return Type Ptr: 0x{:x}", (uintptr_t)this->ffi_return_type_));
        LOGGER.info(std::format("Return Value Ptr: 0x{:x}", (uintptr_t)this->return_value_ptr_));

        /*
         * 开始调用
         */
        ffi_status ffi_prep_status;

        if (func_call_info.isVariadic())
        {
            std::cout << "Preparing variadic cif..." << std::endl;
            ffi_prep_status =
                ffi_prep_cif_var(&this->cif_, FFI_DEFAULT_ABI, func_call_info.getFixedParamCount(),
                                 this->ffi_types_.size(), this->ffi_return_type_, this->ffi_types_.data());
        }
        else
        {
            std::cout << "Preparing normal cif..." << std::endl;
            ffi_prep_status = ffi_prep_cif(&this->cif_, FFI_DEFAULT_ABI, this->ffi_types_.size(),
                                           this->ffi_return_type_, this->ffi_types_.data());
        }

        if (ffi_prep_status != FFI_OK)
        {
            throw std::runtime_error("ffi_prep_cif failed");
        }

        std::cout << "ffi prep cif success" << std::endl;

        ffi_call(&this->cif_, FFI_FN(function_sym), this->return_value_ptr_, ffi_values_.data());
        std::cout << "Return value: " << *static_cast<ffi_arg *>(this->return_value_ptr_) << std::endl;
    }
    // release resources
    free(this->return_value_ptr_);
}
