#include "billing/net/Session.hpp"

#include "billing/net/Packet.hpp"
#include "billing/net/packet/BufferController.hpp"

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
    // Keep session alive
    auto self = this->shared_from_this();

    Packet m_packet;
    m_socket.async_read_some(
      asio::buffer(m_packet.getBuffer()),
      [self, &m_packet](const std::error_code& ec, const std::size_t len)
      {
        std::cout << "Received " << len << " byte(s)" << std::endl;
        m_packet.setSize(len);
        if (ec)
        {
          std::cerr << "Error Code: " << ec << std::endl;
        }
        else
        {
          self->packetHandle(m_packet);
        }
      }
      );
  }

  void Session::packetHandle(const Packet& packet)
  {
    if ((packet.getSize() == 0) || packet.toString().empty())
    {
      std::cerr << "Message empty" << std::endl;
      return;
    }

    // Keep session alive
    auto self = this->shared_from_this();

    std::cout << packet.toString() << std::endl;

    packet::BufferController bufferController(packet.getBuffer());

    asio::async_write(
      m_socket,
      asio::buffer(bufferController.getResponseData()),
      [self](const std::error_code& ec, const std::size_t len)
      {
        std::cout << "Sent " << len << " byte(s)" << std::endl;
        if (ec)
        {
          std::cerr << "Error: " << ec << std::endl;
        }
      }
      );
  }
}

