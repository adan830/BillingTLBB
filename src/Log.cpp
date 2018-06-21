#include "billing/Log.hpp"

#include <spdlog/spdlog.h>
#include <iostream>

#if defined(__linux__)
#  include <sys/stat.h>
#elif defined(__WIN32)
#  include <Windows.h>
#endif

Log::Log() :
  m_folderName("logs"),
  m_fileName("log")
{
  std::cout << "Log system is constructing..." << std::endl;
#if defined(__linux__)
    mkdir(m_folderName.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#elif defined(__WIN32) || defined(WIN32)
    ::CreateDirectory(m_folderName.data(), nullptr)
#endif

  auto dailySink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
    m_folderName + "/" + m_fileName, 23, 59
    );

  spdlog::register_logger(
    std::make_shared<spdlog::logger>("everythings", dailySink)
    );

#if defined(__WIN32)
  auto stdoutSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
  auto stdoutSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

  spdlog::register_logger(
    std::make_shared<spdlog::logger>("console", stdoutSink)
    );

  std::cout << "Log system is constructed!" << std::endl;
}

Log::~Log()
{
  std::cout << "Log system is destructing..." << std::endl;

  spdlog::drop_all();

  std::cout << "Log system is destructed!" << std::endl;
}

std::shared_ptr<Log> Log::getInstance()
{
  static std::shared_ptr<Log> m_instance(new Log());

  return m_instance;
}

