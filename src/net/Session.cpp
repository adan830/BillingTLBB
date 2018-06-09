#include "billing/net/Session.hpp"

#include "billing/net/Packet.hpp"

#include <iostream>
#include <asio.hpp>

namespace net
{
  Session::Session(asio::ip::tcp::socket socket) :
    m_socket(std::move(socket))
  {
  }

  void Session::start()
  {
    Packet m_packet;
    m_socket.async_read_some(
      asio::buffer(m_packet.getBuffer()),
      [this, &m_packet](const std::error_code ec, const std::size_t length)
      {
        std::cout << "Received " << length << " byte(s) with Error Code: " << ec
        << std::endl;

        if (!ec)
        {
          std::cout << m_packet.getBuffer().data() << std::endl;
        }
      }
      );
  }
}

