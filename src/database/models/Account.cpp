#include "billing/database/models/Account.hpp"

namespace database { namespace models {
  Account::Account(const int id) :
    database::Model()
  {
    this->init();
  }

  Account::Account(const std::string& name) :
    database::Model()
  {
    this->init();
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

