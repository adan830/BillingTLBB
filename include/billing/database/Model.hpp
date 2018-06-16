#ifndef __BILLING_DATABASE_MODEL_HPP__
#define __BILLING_DATABASE_MODEL_HPP__

#include <string>

namespace database
{
  class Connector;

  class Model
  {
    private:
      Connector* m_connector;

    protected:
      std::string m_tableName;

    public:
      Model();
      virtual ~Model();

    protected:
      Connector* getConnector() const;

    public:
      const std::string& getTableName() const;
  };
}

#endif

