#include "billing/net/Packet.hpp"

#include "billing/net/packet/HexData.hpp"
#include "billing/Utils.hpp"

namespace net
{
  Packet::Packet(const std::shared_ptr<Buffer> buffer, const std::size_t size) :
    m_buffer(buffer)
  {
    m_hexData = std::shared_ptr<packet::HexData>(
      new packet::HexData(m_buffer, size)
      );

    if (m_hexData->getBody().empty())
    {
      m_size = 0;
    }
    else
    {
      m_size = size;
    }

    m_string.assign(m_buffer->cbegin(), m_buffer->cbegin() + size);

    m_hexString = m_hexData->toString();
  }

  Packet::~Packet()
  {
  }

  std::size_t Packet::getSize() const
  {
    return m_size;
  }

  const std::string& Packet::toString() const
  {
    return m_string;
  }

  std::shared_ptr<packet::HexData> Packet::getHexData() const
  {
    return m_hexData;
  }
}

