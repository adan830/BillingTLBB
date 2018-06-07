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
    asio::io_service m_asioIoService;
    asio::ip::tcp::acceptor* m_acceptor;

  public:
    BillingSocket();
    ~BillingSocket();

  public:
    void start();
    void stop();

  public:
    const std::string& getString() const;
    void setIp(const std::string& ip);

    short getPort() const;
    void setPort(const short port);
};

#endif

