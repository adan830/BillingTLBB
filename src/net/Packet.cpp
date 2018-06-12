#include "billing/net/Packet.hpp"

#include "billing/Utils.hpp"

#include <iostream>

namespace net
{
  Packet::Packet(const std::shared_ptr<Buffer> buffer, const std::size_t size) :
    m_buffer(buffer),
    m_size(size)
  {
  }

  Packet::~Packet()
  {
  }

  std::size_t Packet::getSize() const
  {
    return m_size;
  }

  std::string& Packet::toString() const
  {
    static std::string m_string;
    return m_string.assign(m_buffer->cbegin(), m_buffer->cbegin() + m_size);
  }

  std::string& Packet::toHexString() const
  {
    static std::string m_hexString;

    return m_hexString.assign(Utils::strToHex(
      this->toString().data(), m_size
      ));
  }
}

