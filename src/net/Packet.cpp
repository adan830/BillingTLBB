#include "billing/net/Packet.hpp"

#include "billing/Utils.hpp"

#include <iostream>

namespace net
{
  Packet::Packet(const std::shared_ptr<Buffer> buffer, const std::size_t size) :
    m_buffer(buffer),
    m_size(size)
  {
    m_string.assign(
      m_buffer->cbegin(), m_buffer->cbegin() + m_size
      );

    m_hexString.assign(Utils::strToHex(
        m_string.data(), m_size
        ));
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

  const std::string& Packet::toHexString() const
  {
    return m_hexString;
  }
}

