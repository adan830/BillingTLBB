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
    LOG->warning("BillingSocket is initializing...");

    auto configData = Config::getInstance().getData();

    asio::ip::tcp::endpoint ep(
      asio::ip::address::from_string(configData->ip),
      configData->port
      );

    if (configData->ip == "0.0.0.0")
    {
      ep = asio::ip::tcp::endpoint(
        asio::ip::tcp::v4(),
        configData->port
        );
    }

    m_acceptor = new asio::ip::tcp::acceptor(m_asioIoService, ep);

#if defined(__BILLING_MAX_SESSION__) && (__BILLING_MAX_SESSION__ > 0)
    m_sessionCount = 0;
#endif

    LOG->warning("BillingSocket is initialized");
  }

  BillingSocket::~BillingSocket()
  {
    LOG->warning("BillingSocket is destructing...");

    delete m_acceptor;

    LOG->warning("BillingSocket is destructed!");
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

#if defined(__BILLING_WITH_AFTER_START__)
    this->afterStart();
#endif

    this->accept();

    try
    {
      m_asioIoService.run();
    }
    catch(const std::exception& e)
    {
      LOG->error("Error execepted: {}", e.what());
    }
    catch(...)
    {
      LOG->error("Error execepted at m_asioIoService");
    }
  }

  void BillingSocket::accept()
  {
    LOG->warning("Accepting new connection");

    if (std::ifstream("stop_billing.cmd").good())
    {
      std::remove("stop_billing.cmd");
      LOG->info("Received stop command");
      this->stop();
      return;
    }

    m_acceptor->async_accept(
      m_socket,
      [this](const std::error_code& ec){
        if (ec)
        {
          LOG->error("New connection error code: {}", ec.message());
        }
        else
        {
#if defined(__BILLING_MAX_SESSION__) && (__BILLING_MAX_SESSION__ > 0)
          LOG->warning("Current connection count: {}", m_sessionCount);
          if (m_sessionCount < __BILLING_MAX_SESSION__)
          {
            m_sessionCount++;
#endif
            std::make_shared<Session>(std::move(m_socket))->start();
#if defined(__BILLING_MAX_SESSION__) && (__BILLING_MAX_SESSION__ > 0)
          }
          else
          {
            LOG->info(
              "You can use max {} session(s). Contact: fb.me/dark.hades.1102",
              __BILLING_MAX_SESSION__
              );
          }
#endif
        }
        this->accept();
      }
    );
  }

  void BillingSocket::stop()
  {
    LOG->info("BillingSocket is stopping...");

#if defined(__BILLING_WITH_AFTER_START__)
    /* if (afterStartThread.joinable()) */
    {
      afterStartThread.detach();
#  if defined(BILLING_DEBUG)
      LOG->warning("AfterStartThread is joined");
#  endif
    }
#endif


    m_acceptor->close();
    m_asioIoService.stop();

    LOG->info("BillingSocket is stopped!");
  }
}

