#ifndef __BILLING_DATABASE_MODELS_MAC_ACCOUNT_HPP__
#define __BILLING_DATABASE_MODELS_MAC_ACCOUNT_HPP__

#include "../Model.hpp"

#include <vector>

namespace database { namespace models {
  class MacAccount final : public database::Model
  {
    private:
      int m_id;
      char m_mac[33];
      std::vector<char[33]> m_account;

    public:
      MacAccount();
      ~MacAccount();
  };

} }

