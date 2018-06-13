#ifndef __BILLING_DATABASE_MODELS_ACCOUNT_HPP__
#define __BILLING_DATABASE_MODELS_ACCOUNT_HPP__

#include "../Model.hpp"

namespace database { namespace models {
  class Account : public database::Model
  {
    public:
      Account();
      ~Account();
  };
} }

#endif

