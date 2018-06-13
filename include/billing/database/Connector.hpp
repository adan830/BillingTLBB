#ifndef __BILLING_DATABASE_CONNECTOR_HPP__
#define __BILLING_DATABASE_CONNECTOR_HPP__

#include <mysql.h>
#include <string>

namespace database
{
  class Connector
  {
    private:
      MYSQL* m_connDriver;

    public:
      Connector();
      ~Connector();

    public:
      template<class T>
      T execQuery(const std::string& query);
  };
}

#endif

