#include "billing/BillingSocket.hpp"

#include "billing/Config.hpp"

#include <iostream>

BillingSocket::BillingSocket()
{
  std::cout << "BillingSocket is initializing..." << std::endl;

  auto configData = Config::getInstance().getData();

  std::cout << "BillingSocket is initialized!" << std::endl;
}

BillingSocket::~BillingSocket()
{
  std::cout << "BillingSocket is destructing..." << std::endl;

  std::cout << "BillingSocket is destructed!" << std::endl;
}

