#include "billing/Log.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <iostream>

#if defined(__gnu_linux__)
#  include <sys/stat.h>
#endif

Log::Log() :
  m_folderName("logs"),
  m_fileName("log")
{
#if defined(__gny_linux__)
    if (-1 == mkdir(m_folderName.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
    {
      std::cout << "Error when create log folder" << std::endl;
    }
#endif

  auto dailySink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
    m_folderName + "/" + m_fileName, 23, 59
    );

  spdlog::register_logger(
    std::make_shared<spdlog::logger>("everythings", dailySink)
    );
  // spdlog::register_logger(
  //   std::make_shared<spdlog::stdout_logger_mt>("console")
  //   );
}

Log::~Log()
{
  std::cout << "Log is destructing..." << std::endl;

  spdlog::drop_all();

  std::cout << "Log is destructed" << std::endl;
}

std::shared_ptr<Log> Log::getInstance()
{
  static std::shared_ptr<Log> m_instance(new Log());

  return m_instance;
}

void Log::info(const std::string& log)
{
  spdlog::get("everythings")->info(log);

  spdlog::get("console")->info(log);
}

void Log::warning(const std::string& log)
{
  spdlog::get("everythings")->warn(log);

#if defined(BILLING_DEBUG)
  spdlog::get("console")->warn(log);
#endif
}

void Log::error(const std::string& log)
{
  spdlog::get("everythings")->error(log);

#if defined(BILLING_DEBUG)
  spdlog::get("console")->error(log);
#endif
}

