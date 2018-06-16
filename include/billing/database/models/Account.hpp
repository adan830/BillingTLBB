#ifndef __BILLING_DATABASE_MODELS_ACCOUNT_HPP__
#define __BILLING_DATABASE_MODELS_ACCOUNT_HPP__

#include "../Model.hpp"

namespace database { namespace models {
  class Account : public database::Model
  {
    private:
      int m_id;
      std::string m_name;
      std::string m_password;
      int m_point;

    public:
      explicit Account(const int id);
      explicit Account(const std::string& name);
      virtual ~Account();

    private:
      void init();

    public:
      int getId() const;
      const std::string& getName() const;
      const std::string& getPassword() const;
      int getPoint() const;
  };
} }

#endif

