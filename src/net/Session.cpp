#include "billing/net/Session.hpp"

#include "billing/net/Packet.hpp"

#include <iostream>
#include <asio.hpp>
#include <chrono>
#include <thread>

namespace net
{
  Session::Session(asio::ip::tcp::socket socket) :
    m_socket(std::move(socket))
  {
  }

  Session::~Session()
  {
  }

  void Session::start()
  {
    Packet m_packet;
    m_socket.async_read_some(
      asio::buffer(m_packet.getBuffer()),
      [this, &m_packet](const std::error_code& ec, const std::size_t len)
      {
        std::cout << "Received " << len << " byte(s)" << std::endl;
        if (ec)
        {
          std::cout << "Error Code: " << ec << std::endl;
        }
        else
        {
          std::cout << m_packet.getBuffer().data() << std::endl;
        }
      }
      );
  }
}

