#ifndef __BILLING_NET_SESSION_HPP__
#define __BILLING_NET_SESSION_HPP__

#include <asio.hpp>

namespace net
{
  class Session
  {
    private:
      asio::ip::tcp::socket m_socket;

    public:
      Session(asio::ip::tcp::socket socket);

    public:
      void start();

    public:
      const asio::ip::tcp::socket& getSocket() const;
  };
}

#endif

