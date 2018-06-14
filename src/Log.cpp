#include "billing/Log.hpp"

#include <spdlog/spdlog.h>

Log::Log() :
  m_folderName("logs"),
  m_fileName("runtime.log")
{
}

Log& Log::getInstance()
{
  static Log m_instance;
  return m_instance;
}

void Log::write(const std::string& log)
{
}

#if defined(BILLING_DEBUG)
void Log::debug(const std::string& log)
{
}
#endif

