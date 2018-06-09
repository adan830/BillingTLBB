#include "billing/net/Session.hpp"

#include "billing/net/Packet.hpp"

#include <iostream>
#include <asio.hpp>

namespace net
{
  Session::Session(asio::ip::tcp::socket socket, const Packet& packet) :
    m_socket(std::move(socket)),
    m_packet(packet)
  {
  }

  void Session::start()
  {
    m_socket.async_read_some(
      asio::buffer(m_packet.getBuffer()),
      [this](const std::error_code ec, const std::size_t length)
      {
        std::cout << "Received " << length << " byte(s) with Error Code: " << ec
        << std::endl;
      }
      );
  }
}

