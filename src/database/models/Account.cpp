#include "billing/database/models/Account.hpp"

#include "billing/database/Connector.hpp"
#include "billing/Log.hpp"

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
        point/*,
        is_online,
        is_lock*/
      FROM
        account
      WHERE
        name=?
    )";

    // TODO: Wrap this
    auto conn = this->getConnector();
    // auto ret = conn->query<int, std::string, int>(q, name);

    auto m_connDriver = conn->getConnDriver();
    MYSQL_STMT *stmt = mysql_stmt_init(m_connDriver);
    if (!stmt)
    {
      LOG->error("Error: {}", mysql_error(m_connDriver));
      throw nullptr;
    }
    if (mysql_stmt_prepare(stmt, q, std::strlen(q)))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    MYSQL_BIND bindParams[1];
    std::memset(bindParams, 0, sizeof(bindParams));

    std::size_t l = name.size();
    char c[50];
    std::strcpy(c, name.data());

    bindParams[0].buffer_type = MYSQL_TYPE_STRING;
    bindParams[0].buffer = &c;
    bindParams[0].buffer_length = sizeof(name.size());
    bindParams[0].length = &l;
    bindParams[0].is_null = 0;

    if (mysql_stmt_bind_param(stmt, bindParams))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    auto metaData = mysql_stmt_result_metadata(stmt);
    if (!metaData)
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    auto numFields = mysql_num_fields(metaData);
    if (numFields != 4)
    {
      LOG->error("Num fields is invalid");
      throw nullptr;
    }

    LOG->warning("Params binded");

    MYSQL_BIND bindResult[4];
    unsigned long length[4];
    my_bool is_null[4];
    my_bool error[4];

    std::memset(bindResult, 0, sizeof(bindResult));

    bindResult[0].buffer_type = MYSQL_TYPE_LONG;
    bindResult[0].buffer = (char*)&m_id;
    bindResult[0].length = &length[0];
    bindResult[0].is_null = &is_null[0];
    bindResult[0].error = &error[0];

    bindResult[1].buffer_type = MYSQL_TYPE_STRING;
    bindResult[1].buffer = (char*)&m_name;
    bindResult[1].buffer_length = sizeof(m_name);
    bindResult[1].length = &length[1];
    bindResult[1].is_null = &is_null[1];
    bindResult[1].error = &error[1];

    bindResult[2].buffer_type = MYSQL_TYPE_STRING;
    bindResult[2].buffer = (char*)&m_password;
    bindResult[2].buffer_length = sizeof(m_password);
    bindResult[2].length = &length[2];
    bindResult[2].is_null = &is_null[2];
    bindResult[2].error = &error[2];

    bindResult[3].buffer_type = MYSQL_TYPE_LONG;
    bindResult[3].buffer = (char*)&m_point;
    bindResult[3].length = &length[3];
    bindResult[3].is_null = &is_null[3];
    bindResult[3].error = &error[3];

    if (mysql_stmt_bind_result(stmt, bindResult))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    LOG->warning("Results binded");

    if (mysql_stmt_execute(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    LOG->warning("Executed");

    if (mysql_stmt_store_result(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    LOG->warning("Has result");

    bool found = false;

    while(!mysql_stmt_fetch(stmt))
    {
      LOG->warning(
        "Id: {}, Name: {}, Password: {}, Point: {}",
        m_id, m_name, m_password, m_point
        );
      found = true;
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);

    if (!found)
    {
      LOG->warning("Account [{}] not found", name);
      throw nullptr;
    }
  }

  Account::~Account()
  {
  }

  void Account::init()
  {
    m_tableName = "account";

    m_is_online = false;
    m_is_lock = false;

    m_isPointChanged = false;
    m_isIsLockChanged = false;
    m_isIsOnlineChanged = false;
  }

  int Account::getId() const
  {
    return m_id;
  }

  const char* Account::getName() const
  {
    return m_name;
  }

  const char* Account::getPassword() const
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

