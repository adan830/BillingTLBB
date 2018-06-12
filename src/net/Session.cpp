#include "billing/net/Session.hpp"

#include "billing/net/Packet.hpp"
#include "billing/net/PacketRoutes.hpp"

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

    auto m_buffer = std::make_shared<Packet::Buffer>();
    m_socket.async_receive(
      asio::buffer(*m_buffer),
      [self, m_buffer](const std::error_code& ec, const std::size_t len)
      {
        std::cout << "Received " << len << " byte(s)" << std::endl;
        if (ec)
        {
          std::cerr << "Error Code: " << ec << std::endl;
        }
        else
        {
          auto m_packet = std::make_shared<Packet>(m_buffer, len);
          self->packetHandle(m_packet);
        }
      }
      );
  }

  void Session::packetHandle(std::shared_ptr<Packet> packet)
  {
    if (packet->getSize() == 0)
    {
      std::cerr << "Message empty" << std::endl;
      return;
    }

    // Keep session alive
    auto self = this->shared_from_this();

    std::cout << "Packet Data: " << std::endl;
    std::cout << packet->toString() << std::endl;
    std::cout << packet->toHexString() << std::endl;

    auto responseData = PacketRoutes::getInstance()[packet->toHexString()];

    if (responseData.empty())
    {
      self->start();
      std::cerr << "Notthing to send" << std::endl;
      return;
    }

    m_socket.async_send(
      asio::buffer(responseData),
      [self](const std::error_code& ec, const std::size_t len)
      {
        std::cout << "Sent " << len << " byte(s)" << std::endl;
        if (ec)
        {
          std::cerr << "Error: " << ec << std::endl;
        }
        else
        {
          self->start();
        }
      }
      );
  }

  const asio::ip::tcp::socket& Session::getSocket() const
  {
    return m_socket;
  }
}

