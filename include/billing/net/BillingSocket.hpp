#ifndef __BILLING_BILLING_SOCKET_HPP__
#define __BILLING_BILLING_SOCKET_HPP__

#include <asio.hpp>

#include <string>

namespace net
{
  class Session;

  class BillingSocket final
  {
    private:
      asio::io_service m_asioIoService;
      asio::ip::tcp::acceptor* m_acceptor;
      asio::ip::tcp::socket m_socket;

    public:
      BillingSocket();
      BillingSocket(const std::string& ip, const unsigned short port);
      ~BillingSocket();

    public:
      void start();
      void stop();

    private:
      void accept();

    public:
      const std::string& getString() const;
      void setIp(const std::string& ip);

      unsigned short getPort() const;
      void setPort(const unsigned short port);
  };
}

#endif

