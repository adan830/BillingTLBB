#include "billing/net/Session.hpp"

#include "billing/net/packet/HexData.hpp"
#include "billing/net/packet/Routes.hpp"
#include "billing/net/Packet.hpp"
#include "billing/Utils.hpp"
#include "billing/Log.hpp"

#include <algorithm>
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
          LOG->warning(
            "Raw m_buffer: {}",
            std::string(m_buffer->cbegin(), m_buffer->cbegin() + len)
            );
          LOG->warning(
            "RawHex m_buffer: {}",
            Utils::bytesToHex(m_buffer->data(), len)
            );

          m_queueBuff.insert(
            m_queueBuff.end(), m_buffer->cbegin(), m_buffer->cbegin() + len
            );
          this->queueBufferHandle();

          this->start();
        }
      }
    );
  }

  void Session::queueBufferHandle()
  {
    LOG->warning(
      "Current m_queueBuff Hex: {}",
      Utils::bytesToHex(m_queueBuff.data(), m_queueBuff.size())
      );

    auto headData = Utils::hexToBytes("AA55");
    auto headPos = std::search(
      m_queueBuff.cbegin(), m_queueBuff.cend(),
      headData.cbegin(), headData.cend()
      );
    if (headPos == m_queueBuff.cend())
    {
      LOG->error("m_queueBuff not found head");
      m_queueBuff.clear();
      return;
    }

    auto tailData = Utils::hexToBytes("55AA");
    while (!m_queueBuff.empty())
    {
      if (!this->isConnected())
      {
        break;
      }

      auto buffer = std::make_shared<Packet::Buffer>();
      auto bufferMax = (m_queueBuff.size() > buffer->size())
      ? buffer->size() : m_queueBuff.size();
      std::copy(
        m_queueBuff.cbegin(),
        m_queueBuff.cbegin() + bufferMax,
        buffer->begin()
        );
#if defined(BILLING_DEBUG)
      LOG->warning(
        "Raw buffer: {} - len: {}",
        std::string(buffer->cbegin(), buffer->cend()),
        m_queueBuff.size()
        );
#endif
      auto packet = std::make_shared<Packet>(buffer, bufferMax);
      auto packetSize = packet->getSize();
      auto queueEraseTo = m_queueBuff.begin() + packetSize;
      if (!packetSize)
      {
        auto tailPos = std::search(
          m_queueBuff.begin(), m_queueBuff.end(),
          tailData.cbegin(), tailData.cend()
          );
        if (tailPos == m_queueBuff.cend())
        {
          LOG->warning(
            "Skipping m_queueBuff: {}",
            Utils::bytesToHex(m_queueBuff.data(), m_queueBuff.size())
            );
          break;
        }
        queueEraseTo = tailPos + 2;
      }
      else
      {
        if (!this->packetHandle(packet))
        {
#if defined(BILLING_DEBUG)
          LOG->warning("Notthing to send back");
#endif
        }
      }

      m_queueBuff.erase(
        m_queueBuff.begin(), queueEraseTo
        );

#if defined(BILLING_DEBUG)
      LOG->warning(
        "m_queueBuff is subbed {}",
        Utils::bytesToHex(m_queueBuff.data(), m_queueBuff.size())
        );
#endif
    }
  }

  bool Session::packetHandle(const std::shared_ptr<Packet> packet)
  {
    auto responseData = packet::Routes::getInstance()[packet];

    if (responseData.empty())
    {
      return false;
    }

    LOG->warning(
      "Hex to send: {}",
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

