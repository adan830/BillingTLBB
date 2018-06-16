#ifndef __BILLING_NET_SESSION_HPP__
#define __BILLING_NET_SESSION_HPP__

#include <asio.hpp>
#include <memory>

namespace net
{
  class Packet;

  class Session : public std::enable_shared_from_this<Session>
  {
    private:
      asio::ip::tcp::socket m_socket;

    public:
      Session(asio::ip::tcp::socket socket);
      ~Session();

    public:
      void start();

    protected:
      bool packetHandle(std::shared_ptr<Packet> packet);

    public:
      const asio::ip::tcp::socket& getSocket() const;
  };
}

#endif

