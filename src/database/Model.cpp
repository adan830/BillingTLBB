#include "billing/database/Model.hpp"

#include "billing/database/Connector.hpp"

namespace database
{
  Model::Model()
  {
    LOG->warning("Model is constructing...");
    m_connector = new Connector();
  }

  Model::~Model()
  {
    LOG->warning("Model is destructing...");
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

