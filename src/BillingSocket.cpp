#include "billing/BillingSocket.hpp"

#include "billing/Config.hpp"

#include <iostream>

BillingSocket::BillingSocket()
{
  std::cout << "BillingSocket is initializing..." << std::endl;

  auto configData = Config::getInstance().getData();

  // Construct asio socket
  m_acceptor = new asio::ip::tcp::acceptor(
    m_asioIoService,
    asio::ip::tcp::endpoint(
      asio::ip::address::from_string(configData->ip),
      configData->port
      )
    );

  std::cout << "--- BillingSocket.Acceptor will be listened at "
  << configData->ip
  << ":"
  << configData->port
  << " ---"
  << std::endl;

  std::cout << "BillingSocket is initialized!" << std::endl;
}

BillingSocket::~BillingSocket()
{
  std::cout << "BillingSocket is destructing..." << std::endl;

  delete m_acceptor;

  std::cout << "BillingSocket is destructed!" << std::endl;
}

void BillingSocket::start()
{
  std::cout << "BillingSocket is starting..." << std::endl;

  m_asioIoService.run();

  std::cout << "BillingSocket is started!" << std::endl;
}

void BillingSocket::stop()
{
  std::cout << "BillingSocket is stopping..." << std::endl;

  m_asioIoService.stop();

  std::cout << "BillingSocket is stopped!" << std::endl;
}

