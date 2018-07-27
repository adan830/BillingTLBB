#include "billing/Billing.hpp"

#include "billing/net/BillingSocket.hpp"
#include "billing/Log.hpp"

Billing::Billing() :
  m_socket(new net::BillingSocket())
{
  LOG->warning("Billing is initializing...");

  LOG->warning("Billing is initialized!");
}

Billing::~Billing()
{
  LOG->warning("Billing is destructing...");

  delete m_socket;

  LOG->warning("Billing is destructed!");
}

void Billing::start()
{
  LOG->warning("Billing is starting...");

  m_socket->start();
}

void Billing::stop()
{
  LOG->warning("Billing is stopping...");

  m_socket->stop();

  LOG->info("Billing is stopped!");
}

