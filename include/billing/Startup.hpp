#ifndef __BILLING_STARTUP_HPP__
#define __BILLING_STARTUP_HPP__

#include <unordered_map>
#include <functional>
#include <string>

class Startup final
{
  public:
    using Routers = std::unordered_map<std::string,
          std::function<void(const std::string&)>
          >;

  private:
    std::string m_cmd;
    Routers m_routers;

  public:
    Startup(const int argc, char* argv[]);
    ~Startup();

  public:
    void start();

  private:
    void stop();
};

#endif

