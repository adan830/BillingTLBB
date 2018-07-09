#include "billing/database/Model.hpp"

#include "billing/database/Connector.hpp"
#include "billing/Log.hpp"

namespace database
{
  Model::Model()
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Model is constructing...");
#endif
    m_connector = new Connector();
  }

  Model::~Model()
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Model is destructing...");
#endif
    delete m_connector;
  }

  Connector* Model::getConnector() const
  {
    return m_connector;
  }

  const std::string& Model::getTableName() const
  {
    return m_tableName;
  }
}

