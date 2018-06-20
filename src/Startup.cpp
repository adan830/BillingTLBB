#include "billing/Startup.hpp"

#include "billing/Config.hpp"
#include "billing/Log.hpp"

#include <asio.hpp>
#include <fstream>

Startup::Startup(const int argc, char* argv[])
{
  LOG->warning("Startup is constructing with {} param(s)", argc);

  if (argc == 2)
  {
    m_cmd = argv[1];
  }

  m_routers["stop"] = [this](const std::string&)
  {
    this->stop();
  };

  LOG->warning("Startup is constructed");
}

Startup::~Startup()
{
  LOG->warning("Startup is destructing...");

  LOG->warning("Startup is destructed");
}

void Startup::start()
{
  if (m_cmd.empty())
  {
    return;
  }

  LOG->info("Doing command {}", m_cmd);

  m_routers[m_cmd](m_cmd);

  LOG->info("Executed command: {}", m_cmd);

  throw std::runtime_error("Completed StartUp");
}

void Startup::stop()
{
  std::string stopFileName = "stop_billing.cmd";
  std::fstream stopFile(stopFileName, std::fstream::out);

  if (!stopFile.good())
  {
    LOG->info("Error while creating {}", stopFileName);
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

