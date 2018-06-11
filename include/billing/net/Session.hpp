#ifndef __BILLING_NET_SESSION_HPP__
#define __BILLING_NET_SESSION_HPP__

#include <asio.hpp>
#include <memory>

namespace net
{
  class Session : public std::enable_shared_from_this<Session>
  {
    private:
      asio::ip::tcp::socket m_socket;

    public:
      Session(asio::ip::tcp::socket socket);
      ~Session();

    public:
      void start();

    public:
      const asio::ip::tcp::socket& getSocket() const;
  };
}

#endif

