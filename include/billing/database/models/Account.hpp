#ifndef __BILLING_DATABASE_MODELS_ACCOUNT_HPP__
#define __BILLING_DATABASE_MODELS_ACCOUNT_HPP__

#include "../Model.hpp"

namespace database { namespace models {
  class Account final : public database::Model
  {
    private:
      int m_id;
      char m_name[50];
      char m_password[32];

      int m_point;
      bool m_isPointChanged;

      unsigned short m_is_online;
      bool m_isIsOnlineChanged;

      unsigned short m_is_lock;
      bool m_isIsLockChanged;

    public:
      explicit Account(const std::string& name);
      virtual ~Account();

    private:
      void init();

    public:
      int getId() const;

      const char* getName() const;

      const char* getPassword() const;

      int getPoint() const;
      void setPoint(const int point);

      bool getIsOnline() const;
      void setIsOnline(const bool isOnline);

      bool getIsLock() const;
      void setIsLock(const bool isLock);

    public:
      virtual void save();
  };
} }

#endif

