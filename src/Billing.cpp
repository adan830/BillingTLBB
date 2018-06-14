#include "billing/Billing.hpp"

#include "billing/net/BillingSocket.hpp"
#include "billing/Log.hpp"

Billing::Billing() :
  m_socket(new net::BillingSocket())
{
  LOG->info("Billing is initializing...");

  LOG->info("Billing is initialized!");
}

Billing::~Billing()
{
  LOG->info("Billing is destructing...");

  delete m_socket;

  LOG->info("Billing is destructed!");
}

void Billing::start()
{
  LOG->info("Billing is starting...");

  m_socket->start();

  LOG->info("Billing is started!");
}

void Billing::stop()
{
  LOG->info("Billing is stopping...");

  m_socket->stop();

  LOG->info("Billing is stopped!");
}

