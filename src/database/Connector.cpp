#include "billing/database/Connector.hpp"

#include "billing/Config.hpp"
#include "billing/Log.hpp"

namespace database
{
  Connector::Connector()
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Connector is constructing...");
#endif

    m_connDriver = mysql_init(nullptr);

    this->init();
  }

  Connector::~Connector()
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Database Connector is destructing...");
#endif

    if (m_connDriver)
    {
      mysql_close(m_connDriver);
    }

#if defined(BILLING_DEBUG)
    LOG->warning("Database Connector is destructed");
#endif
  }

  void Connector::init()
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Connector is initializing...");
#endif
    auto configData = Config::getInstance().getData();

    auto c = mysql_real_connect(
      m_connDriver,
      configData->webDatabaseIp.data(),
      configData->webDatabaseUsername.data(),
      configData->webDatabasePassword.data(),
      configData->webDatabaseName.data(),
      configData->webDatabasePort,
      nullptr,
      0
      );

    if (!c)
    {
      // Log to console for notice
      LOG->info("Connector Error: {}", mysql_error(m_connDriver));
      throw nullptr;
    }

#if defined(BILLING_DEBUG)
    LOG->warning("Opened connector database");
#endif
  }

  bool Connector::exec(const char* q)
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Executing bool {}: {}", __FUNCTION__, q);
#endif

    if (mysql_query(m_connDriver, q))
    {
      LOG->error("Error: {}", mysql_error(m_connDriver));
      return false;
    }

    return true;
  }

  MYSQL* Connector::getConnDriver() const
  {
    return m_connDriver;
  }
}

