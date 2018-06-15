#include "billing/net/Session.hpp"

#include "billing/net/packet/Routes.hpp"
#include "billing/net/Packet.hpp"
#include "billing/Log.hpp"

#include <asio.hpp>
#include <chrono>
#include <thread>

namespace net
{
  Session::Session(asio::ip::tcp::socket socket) :
    m_socket(std::move(socket))
  {
    LOG->info(
      "New session {}:{}",
      m_socket.remote_endpoint().address().to_string(),
      m_socket.remote_endpoint().port()
      );
  }

  Session::~Session()
  {
    LOG->info(
      "Session with {}:{} is closed",
      m_socket.remote_endpoint().address().to_string(),
      m_socket.remote_endpoint().port()
      );

    m_socket.close();
  }

  void Session::start()
  {
    // Keep session alive
    auto self = this->shared_from_this();

    auto m_buffer = std::make_shared<Packet::Buffer>();
    m_socket.async_receive(
      asio::buffer(*m_buffer),
      [this, self, m_buffer](const std::error_code& ec, const std::size_t len)
      {
        LOG->warning(
          "Received {} byte(s) from {}:{}",
          len,
          m_socket.remote_endpoint().address().to_string(),
          m_socket.remote_endpoint().port()
          );
        if (ec)
        {
          LOG->error("Socket receive error: {}", ec.message());
        }
        else
        {
          auto m_packet = std::make_shared<Packet>(m_buffer, len);
          if (!self->packetHandle(m_packet))
          {
            self->start();
          }
        }
      }
    );
  }

  bool Session::packetHandle(std::shared_ptr<Packet> packet)
  {
    if (packet->getSize() == 0)
    {
      LOG->error(
        "Message empty from {}:{}",
        m_socket.remote_endpoint().address().to_string(),
        m_socket.remote_endpoint().port()
        );
      return false;
    }

    LOG->warning("Packet Data: {}", packet->toString());
    LOG->warning("Packet Hex: {}", packet->toHexString());

    auto responseData = packet::Routes::getInstance()[packet->toHexString()];

    if (responseData.empty())
    {
      LOG->error(
        "Notthing to send back to {}:{}",
        m_socket.remote_endpoint().address().to_string(),
        m_socket.remote_endpoint().port()
        );
      return false;
    }
    else
    {
      LOG->warning(
        "Sending to {}:{}",
        m_socket.remote_endpoint().address().to_string(),
        m_socket.remote_endpoint().port()
        );
      LOG->warning("Raw data: {}", responseData.data());
    }

    // Keep session alive
    auto self = this->shared_from_this();

    m_socket.async_send(
      asio::buffer(responseData),
      [this, self](const std::error_code& ec, const std::size_t len)
      {
        LOG->warning(
          "Sent {} byte(s) to {}:{}",
          len,
          m_socket.remote_endpoint().address().to_string(),
          m_socket.remote_endpoint().port()
          );
        if (ec)
        {
          LOG->error(
            "Session with {}:{} has error: {}",
            m_socket.remote_endpoint().address().to_string(),
            m_socket.remote_endpoint().port(),
            ec.message()
            );
        }
        else
        {
          self->start();
        }
      }
    );

    return true;
  }

  const asio::ip::tcp::socket& Session::getSocket() const
  {
    return m_socket;
  }
}

