#ifndef __BILLING_NET_SESSION_HPP__
#define __BILLING_NET_SESSION_HPP__

#include <asio.hpp>
#include <memory>

#include "billing/BaseType.hpp"

namespace net
{
  class Packet;

  class Session : public std::enable_shared_from_this<Session>
  {
    private:
      asio::ip::tcp::socket m_socket;
      ByteArray m_queueBuff;

    public:
      Session(asio::ip::tcp::socket socket);
      ~Session();

    public:
      void start();

    protected:
      void queueBufferHandle();
      bool packetHandle(std::shared_ptr<Packet> packet);

    private:
      bool isConnected() const;
  };
}

#endif

