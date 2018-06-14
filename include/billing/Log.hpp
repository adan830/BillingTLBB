#ifndef __BILLING_LOG_HPP__
#define __BILLING_LOG_HPP__

#include <memory>
#include <string>

class Log
{
  private:
    std::string m_folderName;
    std::string m_fileName;
    static Log* s_instance;

  private:
    Log();

  public:
    ~Log();

  public:
    static std::unique_ptr<Log> getInstance();
    void info(const std::string& log);
    void warning(const std::string& log);
    void error(const std::string& log);
};

#define LOG Log::getInstance()

#endif

