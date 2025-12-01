#ifndef LIBCALL_MEMORY_ALLOCATOR_HPP
#define LIBCALL_MEMORY_ALLOCATOR_HPP

#include <cstdlib>
#include <cstring>

#include "adapter/type_mapping.hpp"
#include "types/json_type.hpp"

class MemoryAllocator
{
  public:
    MemoryAllocator() {};
    ~MemoryAllocator() {};

    static void *mallocate(const TypeInfo &type_info, const BaseParamData &base_param_data);
    static void *mallocate(const TypeInfo &type_info, const StructParamData &struct_param_data);

    static void *getDoublePointer(void *data_ptr);

  private:
};

#endif // LIBCALL_MEMORY_ALLOCATOR_HPP
