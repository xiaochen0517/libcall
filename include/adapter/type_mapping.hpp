#ifndef LIBCALL_TYPE_MAPPING_HPP
#define LIBCALL_TYPE_MAPPING_HPP

#include <cstdint>
#include <ffi.h>
#include <map>
#include <string>

struct TypeInfo
{
    size_t type_size;
    ffi_type *ffi_type_ptr;
};

static std::map<std::string, TypeInfo> TYPE_MAP = {{"bool", {sizeof(bool), &ffi_type_uint8}},

                                                   {"schar", {sizeof(signed char), &ffi_type_sint8}},
                                                   {"uchar", {sizeof(unsigned char), &ffi_type_uint8}},

                                                   {"int8", {sizeof(uint8_t), &ffi_type_sint8}},
                                                   {"uint8", {sizeof(uint8_t), &ffi_type_uint8}},
                                                   {"int16", {sizeof(uint16_t), &ffi_type_sint16}},
                                                   {"uint16", {sizeof(uint16_t), &ffi_type_uint16}},
                                                   {"int32", {sizeof(uint32_t), &ffi_type_sint32}},
                                                   {"uint32", {sizeof(uint32_t), &ffi_type_uint32}},
                                                   {"int64", {sizeof(uint64_t), &ffi_type_sint64}},
                                                   {"uint64", {sizeof(uint64_t), &ffi_type_uint64}},

                                                   {"float", {sizeof(float), &ffi_type_float}},
                                                   {"double", {sizeof(double), &ffi_type_double}},
                                                   {"longdouble", {sizeof(long double), &ffi_type_longdouble}},

                                                   {"cstring", {sizeof(char), &ffi_type_pointer}},

                                                   {"pointer", {sizeof(void *), &ffi_type_pointer}},
                                                   {"void", {0, &ffi_type_void}}};

class TypeMapping
{
  public:
    TypeMapping() {};
    ~TypeMapping() {};

    static TypeInfo getTypeInfo(const std::string &type_name);

  private:
};

#endif // LIBCALL_TYPE_MAPPING_HPP
