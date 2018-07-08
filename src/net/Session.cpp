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
      [this, self, m_buffer](const std::error_code& ec, const std::size_t len){
        LOG->warning("Received {} byte(s)", len);
        if (ec)
        {
          LOG->error("Socket received error: {}", ec.message());
        }
        else
        {
          std::string rawString(m_buffer->cbegin(), m_buffer->cbegin()+len);
          LOG->warning(
            "Raw m_buffer: {}",
            rawString
            );
          LOG->warning(
            "RawHex m_buffer: {}",
            Utils::bytesToHex(rawString.data(), rawString.size())
            );
          m_queueBuff.append(rawString);

          while (true)
          {
            if (m_queueBuff.empty())
            {
              break;
            }
            if (!this->isConnected())
            {
              break;
            }

            auto buffer = std::make_shared<Packet::Buffer>();
            std::copy(
              m_queueBuff.cbegin(),
              m_queueBuff.cend(),
              buffer->begin()
              );
            LOG->warning(
              "Raw buffer: {} - len: {}",
              std::string(buffer->cbegin(), buffer->cend()),
              m_queueBuff.size()
              );
            auto packet = std::make_shared<Packet>(buffer, m_queueBuff.size());
            m_queueBuff = m_queueBuff.substr(packet->getSize());
            if (!packet->getSize())
            {
              break;
            }

            if (!this->packetHandle(packet))
            {
              LOG->error("Notthing to send back");
            }
          }
          this->start();
        }
      }
    );
  }

  bool Session::packetHandle(const std::shared_ptr<Packet> packet)
  {
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

    // Keep session alive
    auto self = this->shared_from_this();

    m_socket.async_write_some(
      asio::buffer(responseData),
      [this, self](const std::error_code& ec, const std::size_t len){
        LOG->warning("Sent {} byte(s)", len);
        if (ec)
        {
          m_socket.close();
        }
      });

    return true;
  }

  bool Session::isConnected() const
  {
    std::error_code ec;
    m_socket.remote_endpoint(ec);

    if (ec)
    {
      LOG->error("Error remote_endpoint: {}", ec.message());
      return false;
    }

    return true;
  }
}

