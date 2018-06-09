#ifndef __BILLING_NET_SESSION_HPP__
#define __BILLING_NET_SESSION_HPP__

#include <asio.hpp>

#include "billing/net/Packet.hpp"

namespace net
{
  class Session
  {
    private:
      asio::ip::tcp::socket m_socket;
      Packet m_packet;

    public:
      Session(asio::ip::tcp::socket socket, const Packet& packet = Packet());

    public:
      void start();

    public:
      const asio::ip::tcp::socket* getSocket() const;
      asio::ip::tcp::socket* getSocket();

      const Packet& getPacket() const;
      Packet& getPacket();
  };
}

#endif

