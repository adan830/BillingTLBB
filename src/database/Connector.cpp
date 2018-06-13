#include "billing/database/Connector.hpp"

#include <iostream>

namespace database
{
  Connector::Connector()
  {
    m_connDriver = mysql_init(nullptr);
  }

  Connector::~Connector()
  {
    mysql_close(m_connDriver);
  }

  template<>
  void Connector::execQuery(const std::string& query)
  {
    std::cout << query << std::endl;
  }
}

