#include "billing/database/Connector.hpp"

#include <iostream>

namespace database
{
  Connector::Connector() :
    m_connDriver(mysql_init(nullptr))
  {
  }

  Connector::~Connector()
  {
    std::cout << "Database Connector is destructing..." << std::endl;

    mysql_close(m_connDriver);

    std::cout << "Database Connector is destructed!" << std::endl;
  }

}

