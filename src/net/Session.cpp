#include "billing/net/Session.hpp"

#include "billing/net/packet/HexData.hpp"
#include "billing/net/packet/Routes.hpp"
#include "billing/net/Packet.hpp"
#include "billing/Utils.hpp"
#include "billing/Log.hpp"

#include <asio.hpp>
#include <chrono>
#include <thread>

// Test
#include "billing/database/models/Account.hpp"

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
    database::models::Account a("1");
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
          LOG->warning(
            "Raw: {}",
            std::string(m_buffer->cbegin(), m_buffer->cbegin() + len)
            );
          LOG->warning("RawHex: {}", Utils::bytesToHex(m_buffer->data(), len));
          if (len >= 7)
          {
            self->packetHandle(std::make_shared<Packet>(m_buffer, len));
          }

          self->start();
        }
      }
    );
  }

  bool Session::packetHandle(const std::shared_ptr<Packet> packet)
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

    auto responseData = packet::Routes::getInstance()[packet];

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
      LOG->warning(
        "RawData: {}",
        std::string(
          responseData.cbegin(),
          responseData.cbegin() + responseData.size()
          )
        );
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
      }
      );

    return true;
  }

  const asio::ip::tcp::socket& Session::getSocket() const
  {
    return m_socket;
  }
}

