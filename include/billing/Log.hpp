#ifndef __BILLING_LOG_HPP__
#define __BILLING_LOG_HPP__

#include <string>

class Log
{
  private:
    std::string m_folderName;
    std::string m_fileName;

  private:
    Log();

  public:
    Log& getInstance();
    void write(const std::string& log);
#if defined(BILLING_DEBUG)
    void debug(const std::string& log);
#endif
};

#endif

