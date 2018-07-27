#if 0

#include "billing/database/Connector.hpp"

#include "billing/Log.hpp"

#if (__cplusplus > 201402L) &&  __has_include(<any>)
#  include <any>
#else
#  include "billing/any.hpp"
namespace std
{
  using any = linb::any;
}
#endif

namespace database
{
  template<typename ...TParams>
  bool Connector::exec(const char* q, const TParams&... params)
  {
#if defined(BILLING_DEBUG)
    LOG->warning("Executing {}: {}", __PRETTY_FUNCTION__, q);
#endif

    if (std::strlen(q) == 0)
    {
      LOG->error("Sql query is empty");
      return false;
    }

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
    std::vector<std::any> tParams = {params...};

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

    MYSQL_BIND bind[sizeof...(params)];
    MYSQL_BIND resultBind[sizeof...(TReturn)];

    // std::tuple<const TParams&...> tParams(params...);
    const std::vector<std::any> &tParams = {params...};

    std::vector<std::size_t> bindSizes;
    char *e;

    for (std::size_t i = 0; i < tParams.size(); i++)
    {
      const auto &param = tParams[i];

      LOG->warning("FOR:  {}", param.type().name());

      if (param.type().name() == typeid(std::string).name())
      {
        LOG->warning("TYPE: {}", typeid(std::string).name());
        auto rp = linb::any_cast<std::string>(param);
        bindSizes.emplace_back(rp.size());
        bind[i].buffer_type = MYSQL_TYPE_LONG;
        bind[i].buffer = const_cast<char*>(rp.data());
        bind[i].buffer_length = sizeof(rp.data());
        bind[i].length = (decltype(bind[i].length))bindSizes.at(i);
        bind[i].is_null = 0;
        bind[i].error = nullptr;
        LOG->warning("Binded: {}", rp);

        resultBind[i].buffer_type = MYSQL_TYPE_STRING;
        resultBind[i].buffer = &e;
      }
      else if (param.type().name() == typeid(int).name())
      {
      }
      else if (param.type().name() == typeid(bool).name())
      {
      }
      else if (param.type().name() == typeid(float).name())
      {
      }
    }

    // for (std::size_t i = 0; i < sizeof...(params); i++)
    // {
    //   const auto x = i;
    //   bind[i]->buffer = std::get<x>(tParams);
    // }

    if (mysql_stmt_bind_param(stmt, bind))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    if (mysql_stmt_bind_result(stmt, resultBind))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    if (mysql_stmt_execute(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    if (mysql_stmt_store_result(stmt))
    {
      LOG->error("Error: {}", mysql_stmt_error(stmt));
      throw nullptr;
    }

    while(!mysql_stmt_fetch(stmt))
    {
      LOG->warning("FOUND DATA");
      // printf("6 - %d \n", );
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);

    std::tuple<TReturn...> returnData;

    return returnData;
  }
}

#endif

