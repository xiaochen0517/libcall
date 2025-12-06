#ifndef LIBCALL_TIME_UTIL_HPP
#define LIBCALL_TIME_UTIL_HPP

#pragma once
#include <string>

class TimeUtil
{
  public:
    /**
     * 获取当前时间的字符串表示
     */
    static std::string getCurrentTimeStr();
};

#endif // LIBCALL_TIME_UTIL_HPP
