#include <format>
#include <iostream>
#include <string>

#include "logger/logger.hpp"
#include "utils/time_util.hpp"

Logger &LOGGER = Logger::getInstance();

void Logger::log(const std::string &message, LogLevel level)
{
    std::string prefix;

    if (has_timestamp_)
    {
        std::string time_str = TimeUtil::getCurrentTimeStr();
        prefix = std::format("[{}] ", time_str);
    }

    switch (level)
    {
    case LogLevel::INFO:
        prefix += "[INFO]";
        break;
    case LogLevel::WARN:
        prefix += "[WARN]";
        break;
    case LogLevel::ERROR:
        prefix += "[ERROR]";
        break;
    default:
        prefix += "[UNKNOWN]";
        break;
    }
    if (level != LogLevel::ERROR)
    {
        std::cerr << std::format("{} {}", prefix, message) << std::endl;
    }
    else
    {
        std::cout << std::format("{} {}", prefix, message) << std::endl;
    }
}

void Logger::info(const std::string &message)
{
    log(message, LogLevel::INFO);
}

void Logger::warn(const std::string &message)
{
    log(message, LogLevel::WARN);
}

void Logger::error(const std::string &message)
{
    log(message, LogLevel::ERROR);
}
