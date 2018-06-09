#include "billing/net/Session.hpp"

#include "billing/net/Packet.hpp"

#include <asio.hpp>

namespace net
{
  Session::Session(asio::ip::tcp::socket socket, const Packet& packet) :
    m_socket(std::move(socket)),
    m_packet(packet)
  {
  }
}

