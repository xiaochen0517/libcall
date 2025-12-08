#include <iostream>

int iret_iarg_func(int a, int b)
{
    std::cout << "Call iret_iarg_func with a: " << a << ", b: " << b << std::endl;
    return a + b;
}

int *ipret_iparg_func(int *a, int *b)
{
    std::cout << "Call ipret_iarg_func with a: " << *a << ", b: " << *b << std::endl;
    int *result = new int(*a + *b);
    return result;
}

int &irret_irarg_func(int &a, int &b)
{
    std::cout << "Call irret_iarg_func with a: " << a << ", b: " << b << std::endl;
    static int result;
    result = a + b;
    return result;
}
