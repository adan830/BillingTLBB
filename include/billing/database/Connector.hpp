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
      template<class T, class U = void>
      T execQuery(const std::string& query);
      //
      template<class T>
      T Connector::execQuery<typename std::enable_if<std::is_same<T, std::string>>::type>(const std::string& query)
      {
      }
  };
}

#endif

