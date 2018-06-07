#include "billing/Billing.hpp"

#include "billing/BillingSocket.hpp"

#include <iostream>

Billing::Billing() :
  m_socket(new BillingSocket())
{
  std::cout << "Billing is initializing..." << std::endl;

  std::cout << "Billing is initialized!" << std::endl;
}

Billing::~Billing()
{
  std::cout << "Billing is destructing..." << std::endl;

  delete m_socket;

  std::cout << "Billing is destructed!" << std::endl;
}

void Billing::start()
{
  std::cout << "Billing is starting..." << std::endl;
  m_socket->start();
  std::cout << "Billing is started!" << std::endl;
}

