#ifndef __BILLING_DATABASE_MODEL_HPP__
#define __BILLING_DATABASE_MODEL_HPP__

namespace database
{
  class Connector;

  class Model
  {
    private:
      Connector* m_connector;

    public:
      Model();
      ~Model();

    protected:
      Connector* getConnector() const;
  };
}

#endif

