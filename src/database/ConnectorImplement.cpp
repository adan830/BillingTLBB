#include "billing/database/Connector.hpp"

namespace database {
  template<typename ...TReturn, typename ...TParams>
  std::tuple<TReturn...>
  Connector::select(const std::string& query, TParams... params)
  {

  }
}

