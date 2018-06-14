#include "billing/database/Connector.hpp"

#include "billing/Log.hpp"

namespace database
{
  Connector::Connector() :
    m_connDriver(mysql_init(nullptr))
  {
    this->init();
  }

  Connector::~Connector()
  {
    LOG->info("Database Connector is destructing...");

    mysql_close(m_connDriver);

    LOG->info("Database Connector is destructed");
  }

  void Connector::init()
  {

  }
}

