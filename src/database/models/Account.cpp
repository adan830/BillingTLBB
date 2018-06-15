#include "billing/database/models/Account.hpp"

#include "billing/database/Connector.hpp"

namespace database { namespace models {
  Account::Account(const int id) :
    database::Model()
  {
    this->init();

    std::string q = R"(
      SELECT
        id,
        name,
        point
      FROM
        account
      WHERE
        id=?
    )";

    auto conn = this->getConnector();
    auto ret = conn->query<int, std::string, int>(q.data(), id);
  }

  Account::Account(const std::string& name) :
    database::Model()
  {
    this->init();

    std::string q = R"(
      SELECT
        id,
        name,
        point
      WHERE
        name=?
    )";

    auto conn = this->getConnector();
    auto ret = conn->query<int, std::string, int>(q.data(), name);
  }

  void Account::init()
  {
    m_tableName = "account";

    auto conn = this->getConnector();
    conn->exec("", "");
  }

  int Account::getId() const
  {
    return m_id;
  }

  const std::string& Account::getName() const
  {
    return m_name;
  }

  const std::string& Account::getPassword() const
  {
    return m_password;
  }

  int Account::getPoint() const
  {
    return m_point;
  }
} }

