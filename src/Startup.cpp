#include "billing/Startup.hpp"

#include "billing/Log.hpp"

Startup::Startup(const int argc, char* argv[])
{
  LOG->info("Startup is constructing with {} param(s)", argc);

  if (argc == 2)
  {
    m_cmd = argv[1];
  }

  m_routers["stop"] = [this](const std::string&)
  {
    this->stop();
  };

  LOG->info("Startup is constructed");
}

Startup::~Startup()
{
  LOG->info("Startup is destructing...");

  LOG->info("Startup is destructed");
}

void Startup::start()
{
  if (m_cmd.empty())
  {
    return;
  }

  LOG->info("Doing command {}", m_cmd);

  m_routers[m_cmd](m_cmd);

  throw std::runtime_error("Completed " + m_cmd);
}

void Startup::stop()
{
}

