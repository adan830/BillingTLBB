#ifndef __BILLING_NET_SESSION_HPP__
#define __BILLING_NET_SESSION_HPP__

#include <asio.hpp>

namespace net
{
  class Packet;

  class Session
  {
    private:
      asio::ip::tcp::socket* m_socket;
      const Packet& m_packet;

    public:
      Session(asio::ip::tcp::socket* socket, const Packet& packet);
  };
}

#endif

