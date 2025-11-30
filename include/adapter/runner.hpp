#ifndef LIBCALL_RUNNER_HPP
#define LIBCALL_RUNNER_HPP

#include <ffi.h>
#include <vector>

class Runner
{
  public:
    Runner() {};
    ~Runner() {};

    static Runner create();

    void run();

  private:
    // FFI 参数类型数组
    std::vector<ffi_type *> ffi_types_;
    // FFI 参数值指针数组
    std::vector<void *> ffi_values_;

    ffi_type *ffi_return_type_;
    // 函数返回值
    void *return_value_ptr_;

    ffi_cif cif_;
};

#endif // LIBCALL_RUNNER_HPP
