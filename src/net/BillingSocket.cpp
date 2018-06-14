#include "billing/net/BillingSocket.hpp"

#include "billing/net/Session.hpp"
#include "billing/net/Packet.hpp"
#include "billing/Config.hpp"
#include "billing/Log.hpp"

#include <fstream>

namespace net
{
  BillingSocket::BillingSocket() :
    m_socket(m_asioIoService)
  {
    LOG->info("BillingSocket is initializing...");

    auto configData = Config::getInstance().getData();
    m_acceptor = new asio::ip::tcp::acceptor(
      m_asioIoService,
      asio::ip::tcp::endpoint(
        asio::ip::address::from_string(configData->ip),
        configData->port
        )
      );

    LOG->info("BillingSocket is initialized");
  }

  BillingSocket::BillingSocket(
    const std::string& ip,
    const unsigned short port) :
    m_socket(m_asioIoService)
  {
    LOG->info("BillingSocket is initializing...");

    m_acceptor = new asio::ip::tcp::acceptor(
      m_asioIoService,
      asio::ip::tcp::endpoint(
        asio::ip::address::from_string(ip),
        port
        )
      );

    LOG->info("BillingSocket is initialized");
  }

  BillingSocket::~BillingSocket()
  {
    LOG->info("BillingSocket is destructing...");

    delete m_acceptor;

    LOG->info("BillingSocket is destructed!");
  }

  void BillingSocket::start()
  {
    if (!m_acceptor)
    {
      LOG->info("Acceptor not found");
      return;
    }

    LOG->info("BillingSocket is starting...");

    auto configData = Config::getInstance().getData();

    LOG->info(
      "BillingSocket will be listened at {}:{}",
      configData->ip,
      configData->port
      );

    this->accept();

    m_asioIoService.run();
  }

  void BillingSocket::accept()
  {
    if (std::ifstream("stop_billing.cmd").good())
    {
      std::remove("stop_billing.cmd");
      LOG->info("Received stop command");
      this->stop();
      return;
    }

    m_acceptor->async_accept(
      m_socket,
      [this](const std::error_code& ec)
      {
        if (ec)
        {
          LOG->error("New connection error code: {}", ec.message());
        }
        else
        {
          std::make_shared<Session>(std::move(m_socket))->start();
        }

        this->accept();
      }
      );
  }

  void BillingSocket::stop()
  {
    LOG->info("BillingSocket is stopping...");

    m_acceptor->cancel();
    // m_asioIoService.stop();

    LOG->info("BillingSocket is stopped!");
  }
}

