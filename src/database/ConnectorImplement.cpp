#include "billing/database/Connector.hpp"

#include "billing/Log.hpp"

#include "billing/any.hpp"

namespace database
{
  template<typename ...TParams>
  bool Connector::exec(const char* q, const TParams&... params)
  {
    LOG->warning("Executing: {}", q);

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

    if (mysql_stmt_execute(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      return false;
    }

    MYSQL_BIND bind[sizeof...(params)];

    // std::tuple<const TParams&...> tParams(params...);
    std::vector<linb::any> tParams = {params...};

    for (std::size_t i = 0; i < tParams.size(); i++)
    {
      LOG->info(tParams[i].type().name());
    }

    // for (std::size_t i = 0; i < sizeof...(params); i++)
    // {
    //   const auto x = i;
    //   bind[i]->buffer = std::get<x>(tParams);
    // }

    if (mysql_stmt_bind_param(stmt, bind))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      return false;
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);

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

