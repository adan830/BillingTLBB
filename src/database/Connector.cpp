#include "billing/database/Connector.hpp"

#include <mysql.h>

namespace database
{
  Connector::Connector()
  {
  }

  Connector::~Connector()
  {
  }

  Connector& Connector::getInstance()
  {
    static Connector s_connector;
    return s_connector;
  }
}

