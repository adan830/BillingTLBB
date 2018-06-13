#ifndef __BILLING_DATABASE_CONNECTOR_HPP__
#define __BILLING_DATABASE_CONNECTOR_HPP__

namespace database
{
  struct MYSQL;

  class Connector
  {
    private:
      MYSQL* m_solveDriver;

    public:
      Connector();
      ~Connector();

    public:
      static Connector& getInstance();
  };
}

#endif

