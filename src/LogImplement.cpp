#include "billing/Log.hpp"
#include <spdlog/spdlog.h>

  template<typename T>
void Log::info(const T& log)
{
  spdlog::get("everythings")->info(log);

  spdlog::get("console")->info(log);
}

  template<typename Arg1, typename... Args>
void Log::info(const char* fmt, const Arg1& arg1, const Args& ...args)
{
  spdlog::get("everythings")->info(fmt, arg1, args...);

  spdlog::get("console")->info(fmt, arg1, args...);
}

  template<typename T>
void Log::warning(const T& log)
{
  spdlog::get("everythings")->warn(log);

#if defined(BILLING_DEBUG)
  spdlog::get("console")->warn(log);
#endif
}

  template<typename Arg1, typename... Args>
void Log::warning(const char* fmt, const Arg1& arg1, const Args& ...args)
{
  spdlog::get("everythings")->warn(fmt, arg1, args...);

#if defined(BILLING_DEBUG)
  spdlog::get("console")->warn(fmt, arg1, args...);
#endif
}

  template<typename T>
void Log::error(const T& log)
{
  spdlog::get("everythings")->error(log);

#if defined(BILLING_DEBUG)
  spdlog::get("console")->error(log);
#endif
}

  template<typename Arg1, typename... Args>
void Log::error(const char* fmt, const Arg1& arg1, const Args& ...args)
{
  spdlog::get("everythings")->error(fmt, arg1, args...);

#if defined(BILLING_DEBUG)
  spdlog::get("console")->error(fmt, arg1, args...);
#endif
}

