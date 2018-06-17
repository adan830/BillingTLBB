#include "billing/database/models/Account.hpp"

#include "billing/database/Connector.hpp"

namespace database { namespace models {
  Account::Account(const std::string& name) :
    database::Model()
  {
    LOG->warning("Account is constructing with name: {}", name);

    this->init();

    constexpr auto q = R"(
      SELECT
        id,
        name,
        password,
        point,
        is_online,
        is_lock
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

    m_isPointChanged = false;
    m_isIsLockChanged = false;
    m_isIsOnlineChanged = false;
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

  void Account::setPoint(const int point)
  {
    m_point = point;
    m_isPointChanged = true;
  }

  bool Account::getIsOnline() const
  {
    return m_is_online;
  }

  void Account::setIsOnline(const bool isOnline)
  {
    m_point = isOnline;
    m_isPointChanged = true;
  }

  bool Account::getIsLock() const
  {
    return m_is_lock;
  }

  void Account::setIsLock(const bool isLock)
  {
    m_is_lock = isLock;
    m_isIsLockChanged = true;
  }

  void Account::save()
  {
    LOG->warning("Saving account: {}", m_name);

    LOG->warning("Saving account: {} completed", m_name);
  }
} }

