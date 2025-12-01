#ifndef LIBCALL_TYPE_HPP
#define LIBCALL_TYPE_HPP

#include <ffi.h>
#include <string>
#include <vector>

/**
 * 调用类型
 */
class LCTypeInfo
{
  public:
    LCTypeInfo(std::string type_name_, std::string label_name_, ffi_type *ffi_type_ptr_, void *data_ptr_, void *double_data_ptr_)
        : type_name_(type_name_), label_name_(label_name_), ffi_type_ptr_(ffi_type_ptr_), data_ptr_(data_ptr_), double_data_ptr_(double_data_ptr_) {};
    ~LCTypeInfo(){};

    std::string getTypeName() const
    {
        return type_name_;
    };
    std::string getLabelName() const
    {
        return label_name_;
    };
    ffi_type *getFFITypePtr() const
    {
        return ffi_type_ptr_;
    };
    void *getDataPtr() const
    {
        return data_ptr_;
    };
    void *getDoubleDataPtr() const
    {
        return double_data_ptr_;
    };

  private:
    std::string type_name_;
    std::string label_name_;
    ffi_type *ffi_type_ptr_;
    void *data_ptr_;
    void *double_data_ptr_;
};

class LCStructTypeInfo
{
  public:
    LCStructTypeInfo() {};
    ~LCStructTypeInfo()
    {
        for (void *data_ptr : data_ptr_test_)
        {
            if (data_ptr)
            {
                free(data_ptr);
            }
        }
    };

    std::string getTypeName() const
    {
        return type_name_;
    };
    std::string getLabelName() const
    {
        return label_name_;
    };

  private:
    std::string type_name_;
    std::string label_name_;
    std::vector<ffi_type *> ffi_type_ptr_list_;
    std::vector<void *> data_ptr_test_;
};

#endif // LIBCALL_TYPE_HPP
