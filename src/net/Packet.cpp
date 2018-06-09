#include "billing/net/Packet.hpp"

namespace net
{
  Packet::Packet()
  {
  }

  Packet::~Packet()
  {
  }

  void Packet::setBuffer(const Buffer& buffer)
  {
    m_buffer = buffer;
  }

  const Packet::Buffer& Packet::getBuffer() const
  {
    return m_buffer;
  }

  Packet::Buffer& Packet::getBuffer()
  {
    return m_buffer;
  }
}

