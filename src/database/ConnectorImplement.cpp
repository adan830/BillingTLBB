#include "billing/database/Connector.hpp"

#include "billing/Log.hpp"

namespace database {
  template<typename ...TParams>
  inline bool Connector::exec(const char* q, const TParams&... params)
  {
    LOG->warning("Executing: {}", q);

    MYSQL_STMT *stmt = mysql_stmt_init(m_connDriver);
    if (!stmt)
    {
      LOG->error("Error: {}", mysql_error(m_connDriver));
      return false;
    }

    return true;
  }

  template<typename ...TReturn, typename ...TParams>
  inline std::tuple<TReturn...>
  Connector::query(
    const char* q,
    const TParams&... params
    )
  {
    LOG->warning("Executing: {}", q);

    std::tuple<TReturn...> returnData;

    return returnData;
  }
}

