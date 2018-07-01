#include "billing/net/Session.hpp"

#include "billing/net/packet/HexData.hpp"
#include "billing/net/packet/Routes.hpp"
#include "billing/net/Packet.hpp"
#include "billing/Utils.hpp"
#include "billing/Log.hpp"

#include <asio.hpp>
#include <future>

namespace net
{
  Session::Session(asio::ip::tcp::socket socket) :
    m_socket(std::move(socket))
  {
    LOG->info(
      "New session from {}:{}",
      m_socket.remote_endpoint().address().to_string(),
      m_socket.remote_endpoint().port()
      );
  }

  Session::~Session()
  {
    LOG->info("A session is being close");

    m_socket.close();
  }

  void Session::start()
  {
    if (!this->isConnected())
    {
      return;
    }

    LOG->warning("Session read starting");

    // Keep session alive
    auto self = this->shared_from_this();

    auto m_buffer = std::make_shared<Packet::Buffer>();

    m_socket.async_receive(
      asio::buffer(*m_buffer),
      [this, self, m_buffer](const std::error_code& ec, const std::size_t len)
      {
        LOG->warning("Received {} byte(s)", len);
        if (ec)
        {
          LOG->error("Socket received error: {}", ec.message());
        }
        else
        {
          LOG->warning(
            "Raw: {}",
            std::string(m_buffer->cbegin(), m_buffer->cbegin() + len)
            );

          LOG->warning("RawHex: {}", Utils::bytesToHex(m_buffer->data(), len));

          std::async([this, self, m_buffer, len](){
            this->packetHandle(std::make_shared<Packet>(m_buffer, len));
          });

          this->start();
        }
      }
    );
  }

  bool Session::packetHandle(const std::shared_ptr<Packet> packet)
  {
    if (!this->isConnected())
    {
      return false;
    }

    if (packet->getSize() == 0)
    {
      return false;
    }

    // Keep session alive
    auto self = this->shared_from_this();

    auto responseData = packet::Routes::getInstance()[packet];

    if (responseData.empty())
    {
      return false;
    }

    LOG->warning(
      "RawData to send: {} - Hex: {}",
      std::string(
        responseData.cbegin(),
        responseData.cbegin() + responseData.size()
        ),
      Utils::bytesToHex(responseData.data(), responseData.size())
      );

    asio::async_write(
      m_socket, asio::buffer(responseData),
      [this, self](const std::error_code& ec, const std::size_t len)
      {
        LOG->warning("Sent {} byte(s)", len);
        if (ec)
        {
          m_socket.close();
        }
      }
      );

    return true;
  }

  bool Session::isConnected() const
  {
    std::error_code ec;
    m_socket.remote_endpoint(ec);

    if (ec)
    {
      return false;
    }

    return true;
  }
}

