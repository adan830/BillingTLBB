#include "billing/net/Packet.hpp"

#include "billing/net/packet/HexData.hpp"
#include "billing/Utils.hpp"

namespace net
{
  Packet::Packet(const std::shared_ptr<Buffer> buffer, const std::size_t size) :
    m_buffer(buffer)
  {
    m_hexData = std::make_shared<packet::HexData>(m_buffer, size);

    if (m_hexData->getBody().size())
    {
      m_hexString = m_hexData->toString();
      m_string.assign(
        m_buffer->cbegin(),
        m_buffer->cbegin() + m_hexString.size() / 2
        );
    }
  }

  Packet::~Packet()
  {
  }

  std::size_t Packet::getSize() const
  {
    return m_string.size();
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

