#ifndef __BILLING_BILLING_SOCKET_HPP__
#define __BILLING_BILLING_SOCKET_HPP__

#define ASIO_STANDALONE
#define ASIO_HAS_STD_CHRONO

#include <asio.hpp>
#include <string>

class BillingSocket final
{
  private:
    std::string m_ip;
    short m_port;

  public:
    BillingSocket(const std::string& ip, const short port);

  public:
    const std::string& getString() const;
    void setIp(const std::string& ip);

    short getPort() const;
    void setPort(const short port);
};

#endif

