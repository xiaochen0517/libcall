#ifndef LIBCALL_LOGGER_HPP
#define LIBCALL_LOGGER_HPP

#pragma once
#include <string>

class Logger
{
  public:
    static Logger &getInstance()
    {
        static Logger instance;
        return instance;
    }

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    enum class LogLevel
    {
        INFO,
        WARN,
        ERROR
    };

    void log(const std::string &message, LogLevel level = LogLevel::INFO);

    void info(const std::string &message);
    void warn(const std::string &message);
    void error(const std::string &message);

  private:
    Logger() = default;

    bool has_timestamp_ = true;
};

extern Logger &LOGGER;

#endif // LIBCALL_LOGGER_HPP
