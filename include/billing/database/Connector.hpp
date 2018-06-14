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

    private:
      void init();

    public:
      template<typename ...TReturn, typename ...TParams>
      std::tuple<TReturn...> select(const std::string& query, TParams... params);
  };
}

#include "../../../src/database/ConnectorImplement.cpp"

#endif

