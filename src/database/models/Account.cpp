#include "billing/database/models/Account.hpp"

#include "billing/database/Connector.hpp"
#include "billing/Log.hpp"

namespace database { namespace models {
  Account::Account(const std::string& name) :
    database::Model()
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Account is constructing with name: {}", name);
#endif

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
    // auto ret = conn->query<int, std::string, int>(q, name);

    // TODO: Wrap this bellow
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

    unsigned long l = name.size();
    char c[30];
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

    constexpr std::size_t numFields = 6;
    auto stmtNumFields = mysql_num_fields(metaData);
    if (stmtNumFields != numFields)
    {
      LOG->error("Num fields is invalid");
      throw nullptr;
    }

#if defined(BILLING_DEBUG)
    LOG->warning("Params binded");
#endif

    MYSQL_BIND bindResult[numFields];
    unsigned long length[numFields];
    my_bool is_null[numFields];
    my_bool error[numFields];

    std::memset(bindResult, 0, sizeof(bindResult));

    bindResult[0].buffer_type = MYSQL_TYPE_LONG;
    bindResult[0].buffer = &m_id;
    bindResult[0].length = &length[0];
    bindResult[0].is_null = &is_null[0];
    bindResult[0].error = &error[0];

    bindResult[1].buffer_type = MYSQL_TYPE_STRING;
    bindResult[1].buffer = &m_name;
    bindResult[1].buffer_length = sizeof(m_name);
    bindResult[1].length = &length[1];
    bindResult[1].is_null = &is_null[1];
    bindResult[1].error = &error[1];

    bindResult[2].buffer_type = MYSQL_TYPE_STRING;
    bindResult[2].buffer = &m_password;
    bindResult[2].buffer_length = sizeof(m_password);
    bindResult[2].length = &length[2];
    bindResult[2].is_null = &is_null[2];
    bindResult[2].error = &error[2];

    bindResult[3].buffer_type = MYSQL_TYPE_LONG;
    bindResult[3].buffer = &m_point;
    bindResult[3].length = &length[3];
    bindResult[3].is_null = &is_null[3];
    bindResult[3].error = &error[3];

    bindResult[4].buffer_type = MYSQL_TYPE_TINY;
    bindResult[4].buffer = &m_is_online;
    bindResult[4].length = &length[4];
    bindResult[4].is_null = &is_null[4];
    bindResult[4].error = &error[4];

    bindResult[5].buffer_type = MYSQL_TYPE_TINY;
    bindResult[5].buffer = &m_is_lock;
    bindResult[5].length = &length[4];
    bindResult[5].is_null = &is_null[4];
    bindResult[5].error = &error[4];

    if (mysql_stmt_bind_result(stmt, bindResult))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

#if defined(BILLING_DEBUG)
    LOG->warning("Results binded");
#endif

    if (mysql_stmt_execute(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

#if defined(BILLING_DEBUG)
    LOG->warning("Executed");
#endif

    if (mysql_stmt_store_result(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

#if defined(BILLING_DEBUG)
    LOG->warning("Has result");
#endif

    while(!mysql_stmt_fetch(stmt))
    {
      LOG->warning(
        "Id={}, Name={}, Password={}, Point={}, Online={}, Lock={}",
        m_id, m_name, m_password, m_point, m_is_online, m_is_lock
        );
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);
  }

  Account::~Account()
  {
  }

  void Account::init()
  {
    m_tableName = "account";

    m_is_online = 0;
    m_is_lock = 0;

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
    // m_isPointChanged = true;
  }

  bool Account::getIsOnline() const
  {
    return m_is_online;
  }

  void Account::setIsOnline(const bool isOnline)
  {
    m_is_online = static_cast<unsigned short>(isOnline);
    // m_isPointChanged = true;
  }

  bool Account::getIsLock() const
  {
    return m_is_lock;
  }

  void Account::setIsLock(const bool isLock)
  {
    m_is_lock = static_cast<unsigned short>(isLock);
    // m_isIsLockChanged = true;
  }

  bool Account::save()
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Saving account: {}", m_name);
#endif

    constexpr auto q = R"(
      UPDATE
        account
      SET
        is_online=?,
        point=?
      WHERE
        id=?
    )";
#if defined(BILLING_DEBUG)
    LOG->warning("SQL: {}", q);
    LOG->warning("Params: {}, {}, {}", m_is_online, m_point, m_id);
#endif

    auto conn = this->getConnector();

    // TODO: Wrap this bellow
    auto m_connDriver = conn->getConnDriver();
    MYSQL_STMT *stmt = mysql_stmt_init(m_connDriver);
    if (!stmt)
    {
      LOG->error("Error: {}", mysql_error(m_connDriver));
      return false;
    }

    if (mysql_stmt_prepare(stmt, q, std::strlen(q)))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      return false;
    }

    constexpr std::size_t bindParamNums = 3;
    MYSQL_BIND bindParams[bindParamNums];
    std::memset(bindParams, 0, sizeof(bindParams));

    bindParams[0].buffer_type = MYSQL_TYPE_TINY;
    bindParams[0].buffer = &m_is_online;
    bindParams[0].buffer_length = sizeof(m_is_online);
    bindParams[0].is_null = 0;
    bindParams[0].error = nullptr;

    bindParams[1].buffer_type = MYSQL_TYPE_LONG;
    bindParams[1].buffer = &m_point;
    bindParams[1].buffer_length = sizeof(m_point);
    bindParams[1].is_null = 0;
    bindParams[1].error = nullptr;

    bindParams[2].buffer_type = MYSQL_TYPE_LONG;
    bindParams[2].buffer = &m_id;
    bindParams[2].buffer_length = sizeof(m_id);
    bindParams[2].is_null = 0;
    bindParams[2].error = nullptr;

    if (mysql_stmt_bind_param(stmt, bindParams))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      return false;
    }

    if (mysql_stmt_execute(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      return false;
    }

#if defined(BILLING_DEBUG)
    LOG->warning("Saved account: {}", m_name);
#endif

    return true;
  }
} }

