#include "billing/Startup.hpp"

#include "billing/Config.hpp"
#include "billing/Log.hpp"

#include <asio.hpp>
#include <fstream>

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

  throw std::runtime_error("Completed command: " + m_cmd);
}

void Startup::stop()
{
  std::fstream stopFile("stop_billing.cmd", std::fstream::out);

  if (!stopFile.good())
  {
    LOG->info("Error while creating stop_billing.cmd");
    return;
  }
  stopFile.close();

  auto configData = Config::getInstance().getData();
  asio::io_service m_asioIoService;
  asio::ip::tcp::socket m_socket(m_asioIoService);
  m_socket.connect(asio::ip::tcp::endpoint(
      asio::ip::address::from_string(configData->ip),
      configData->port
      ));
  m_socket.send(asio::buffer("stop"));
  m_socket.close();
  m_asioIoService.run();
}

