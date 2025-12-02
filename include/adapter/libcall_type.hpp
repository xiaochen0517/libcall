#ifndef LIBCALL_TYPE_HPP
#define LIBCALL_TYPE_HPP

#include <ffi.h>
#include <string>
#include <vector>

/**
 * 调用类型
 */
class LCBaseTypeInfo
{
  public:
    LCBaseTypeInfo(std::string type_name_, std::string label_name_, ffi_type *ffi_type_ptr_, void *data_ptr_,
                   void *double_data_ptr_)
        : type_name_(type_name_), label_name_(label_name_), ffi_type_ptr_(ffi_type_ptr_), data_ptr_(data_ptr_),
          double_data_ptr_(double_data_ptr_) {};
    ~LCBaseTypeInfo() {};

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

class LCLibInfo
{
  public:
    LCLibInfo(std::string label_name_, std::string lib_path_, void *lib_handle_)
        : label_name_(label_name_), lib_path_(lib_path_), lib_handle_(lib_handle_) {};
    ~LCLibInfo() {};

    std::string getLabelName() const
    {
        return label_name_;
    };
    std::string getLibPath() const
    {
        return lib_path_;
    };
    void *getLibHandle() const
    {
        return lib_handle_;
    };

  private:
    std::string label_name_;
    std::string lib_path_;
    void *lib_handle_;
};

class LCFuncCallInfo
{
  public:
    LCFuncCallInfo(std::string label_name_, std::string lib_label_, std::string func_name_,
                   std::vector<std::string> param_labels_, std::string return_label_)
        : label_name_(label_name_), lib_label_(lib_label_), func_name_(func_name_), param_labels_(param_labels_),
          return_label_(return_label_) {};
    ~LCFuncCallInfo() {};

    std::string getLabelName() const
    {
        return label_name_;
    };
    std::string getLibLabel() const
    {
        return lib_label_;
    };
    std::string getFuncName() const
    {
        return func_name_;
    };
    std::vector<std::string> getParamLabels() const
    {
        return param_labels_;
    };
    std::string getReturnLabel() const
    {
        return return_label_;
    };

  private:
    std::string label_name_;
    std::string lib_label_;
    std::string func_name_;
    std::vector<std::string> param_labels_;
    std::string return_label_;
};

#endif // LIBCALL_TYPE_HPP
