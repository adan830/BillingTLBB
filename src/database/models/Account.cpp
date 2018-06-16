#include "billing/database/models/Account.hpp"

#include "billing/database/Connector.hpp"

namespace database { namespace models {
  Account::Account(const int id) :
    database::Model()
  {
    LOG->warning("Account is constructing with id: {}", id);

    this->init();

    constexpr auto q = R"(
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
    auto ret = conn->query<int, std::string, int>(q, id);
  }

  Account::Account(const std::string& name) :
    database::Model()
  {
    LOG->warning("Account is constructing with name: {}", name);

    this->init();

    constexpr auto q = R"(
      SELECT
        id,
        name,
        point
      FROM
        account
      WHERE
        name=?
    )";

    auto conn = this->getConnector();
    auto ret = conn->query<int, std::string, int>(q, name);
  }

  Account::~Account()
  {
  }

  void Account::init()
  {
    m_tableName = "account";
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

