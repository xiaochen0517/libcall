#include <chrono>

#include "utils/time_util.hpp"

std::string TimeUtil::getCurrentTimeStr()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char time_buffer[20];
    std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
    return std::string(time_buffer);
}
