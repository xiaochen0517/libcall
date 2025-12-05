#include <cstdint>
#include <iostream>

struct Rectangle
{
    uint32_t width;
    uint32_t height;
};

static Rectangle global_rect = {100, 100};

/**
 * 普通类型无参无返回值
 */

Rectangle sret_narg_func()
{
    std::cout << "Call struct_noarg_func " << std::endl;
    return Rectangle{10, 20};
}

void nret_sarg_func(Rectangle rect)
{
    std::cout << "Call noret_sarg_func ";
    std::cout << "width: " << rect.width << ", height: " << rect.height << std::endl;
}

Rectangle sret_sarg_func(Rectangle rect)
{
    std::cout << "Call sret_sarg_func ";
    std::cout << "width: " << rect.width << ", height: " << rect.height << std::endl;
    return Rectangle{rect.width + 1, rect.height + 1};
}

/**
 * 引用类型参数和返回值
 */

Rectangle &srret_narg_func()
{
    std::cout << "Call srret_narg_func " << std::endl;
    return global_rect;
}

void nret_srag_func(Rectangle &rect)
{
    std::cout << "Call noret_srag_func ";
    std::cout << "width: " << rect.width << ", height: " << rect.height << std::endl;
}

Rectangle &srret_srag_func(Rectangle &rect)
{
    std::cout << "Call srret_srag_func ";
    std::cout << "width: " << rect.width << ", height: " << rect.height << std::endl;
    rect.width += 1;
    rect.height += 1;
    return rect;
}

/**
 * 指针类型参数和返回值
 */

Rectangle *spret_narg_func()
{
    std::cout << "Call struct_ptr_noarg_func " << std::endl;
    Rectangle *rect = new Rectangle{30, 40};
    return rect;
}

void nret_sparg_func(Rectangle *rect)
{
    std::cout << "Call noret_struct_ptr_arg_func ";
    std::cout << "width: " << rect->width << ", height: " << rect->height << std::endl;
}

Rectangle *spret_sparg_func(Rectangle *rect)
{
    std::cout << "Call struct_func_ptr ";
    std::cout << "width: " << rect->width << ", height: " << rect->height << std::endl;
    rect->width += 1;
    rect->height += 1;
    return rect;
}
