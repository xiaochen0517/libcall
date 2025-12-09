#include <iostream>
#include <cstdint>

/**
 * uint8_t functions
 */

uint8_t i8ret_i8arg_func(uint8_t a, uint8_t b)
{
    std::cout << "Call iret_iarg_func with a: " << a << ", b: " << b << std::endl;
    return a + b;
}

uint8_t *i8pret_i8parg_func(uint8_t *a, uint8_t *b)
{
    std::cout << "Call ipret_iarg_func with a: " << *a << ", b: " << *b << std::endl;
    uint8_t *result = new uint8_t(*a + *b);
    return result;
}

uint8_t &i8rret_i8rarg_func(uint8_t &a, uint8_t &b)
{
    std::cout << "Call irret_iarg_func with a: " << a << ", b: " << b << std::endl;
    static uint8_t result;
    result = a + b;
    return result;
}

/**
 * uint16_t functions
 */

uint16_t i16ret_i16arg_func(uint16_t a, uint16_t b)
{
    std::cout << "Call iret_iarg_func with a: " << a << ", b: " << b << std::endl;
    return a + b;
}

uint16_t *i16pret_i16parg_func(uint16_t *a, uint16_t *b)
{
    std::cout << "Call ipret_iarg_func with a: " << *a << ", b: " << *b << std::endl;
    uint16_t *result = new uint16_t(*a + *b);
    return result;
}

uint16_t &i16rret_i16rarg_func(uint16_t &a, uint16_t &b)
{
    std::cout << "Call irret_iarg_func with a: " << a << ", b: " << b << std::endl;
    static uint16_t result;
    result = a + b;
    return result;
}

/**
 * uint32_t functions
 */

uint32_t i32ret_i32arg_func(uint32_t a, uint32_t b)
{
    std::cout << "Call iret_iarg_func with a: " << a << ", b: " << b << std::endl;
    return a + b;
}

uint32_t *i32pret_i32parg_func(uint32_t *a, uint32_t *b)
{
    std::cout << "Call ipret_iarg_func with a: " << *a << ", b: " << *b << std::endl;
    uint32_t *result = new uint32_t(*a + *b);
    return result;
}

uint32_t &i32rret_i32rarg_func(uint32_t &a, uint32_t &b)
{
    std::cout << "Call irret_iarg_func with a: " << a << ", b: " << b << std::endl;
    static uint32_t result;
    result = a + b;
    return result;
}



/**
 * uint64_t functions
 */

uint64_t i64ret_i64arg_func(uint64_t a, uint64_t b)
{
    std::cout << "Call iret_iarg_func with a: " << a << ", b: " << b << std::endl;
    return a + b;
}

uint64_t *i64pret_i64parg_func(uint64_t *a, uint64_t *b)
{
    std::cout << "Call ipret_iarg_func with a: " << *a << ", b: " << *b << std::endl;
    uint64_t *result = new uint64_t(*a + *b);
    return result;
}

uint64_t &i64rret_i64rarg_func(uint64_t &a, uint64_t &b)
{
    std::cout << "Call irret_iarg_func with a: " << a << ", b: " << b << std::endl;
    static uint64_t result;
    result = a + b;
    return result;
}

/**
 * int functions
 */

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