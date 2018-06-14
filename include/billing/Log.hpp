#ifndef __BILLING_LOG_HPP__
#define __BILLING_LOG_HPP__

#include <memory>
#include <string>

class Log
{
  private:
    std::string m_folderName;
    std::string m_fileName;

  private:
    Log();

  public:
    ~Log();

  public:
    static std::shared_ptr<Log> getInstance();

    template<typename T>
    void info(const T& log);

    template<typename Arg1, typename... Args>
    void info(const char* fmt, const Arg1& arg1, const Args& ...args);

    template<typename T>
    void warning(const T& log);

    template<typename Arg1, typename... Args>
    void warning(const char* fmt, const Arg1& arg1, const Args& ...args);

    template<typename T>
    void error(const T& log);

    template<typename Arg1, typename... Args>
    void error(const char* fmt, const Arg1& arg1, const Args& ...args);
};

#include "../../src/LogImplement.cpp"

#define LOG Log::getInstance()

#endif

