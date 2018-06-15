#include "billing/database/Connector.hpp"

#include "billing/Config.hpp"
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

    if (m_connDriver)
    {
      mysql_close(m_connDriver);
    }

    LOG->info("Database Connector is destructed");
  }

  void Connector::init()
  {
    auto configData = Config::getInstance().getData();

    auto c = mysql_real_connect(
      m_connDriver,
      configData->webDatabaseIp.data(),
      configData->webDatabaseUsername.data(),
      configData->webDatabasePassword.data(),
      configData->webDatabaseName.data(),
      configData->webDatabasePort,
      NULL,
      0
      );

    if (!c)
    {
      throw std::runtime_error(mysql_error(m_connDriver));
    }
  }

  bool Connector::exec(const char* q)
  {
    LOG->warning("Executing: {}", q);

    if (mysql_query(m_connDriver, q))
    {
      LOG->error("Error: {}", mysql_error(m_connDriver));
      return false;
    }

    return true;
  }
}

