#ifndef __BILLING_DATABASE_CONNECTOR_HPP__
#define __BILLING_DATABASE_CONNECTOR_HPP__

#include <mysql.h>
#include <string>
#include <tuple>

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
      bool exec(const char* q);

      template<typename ...TParams>
      bool exec(const char* q, const TParams&... params);

      template<typename ...TReturn, typename ...TParams>
      std::tuple<TReturn...> query(
        const char* q,
        const TParams&... params
        );
  };
}

#include "../../../src/database/ConnectorImplement.cpp"

#endif

