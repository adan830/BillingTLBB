#include "billing/database/Connector.hpp"

#include "billing/Config.hpp"
#include "billing/Log.hpp"

namespace database
{
  Connector::Connector()
  {
    LOG->warning("Connector is constructing...");

    m_connDriver = mysql_init(nullptr);

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
    LOG->warning("Connector is initializing...");
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
      LOG->error("Connector Error: {}", mysql_error(m_connDriver));
      throw nullptr;
    }

    LOG->warning("Opened connector database");
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

