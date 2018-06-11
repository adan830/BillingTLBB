#include "billing/net/Packet.hpp"

namespace net
{
  Packet::Packet() :
    m_size(0)
  {
  }

  Packet::Packet(const std::size_t size) :
    m_size(size)
  {
  }

  Packet::Packet(const Buffer& buffer) :
    m_buffer(buffer)
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

  void Packet::setSize(const std::size_t size)
  {
    m_size = (m_size == 0) ? size : m_size;
  }

  std::size_t Packet::getSize() const
  {
    return m_size;
  }

  const std::string& Packet::toString() const
  {
    static std::string m_string;

    return m_string.assign(m_buffer.cbegin(), m_buffer.cbegin() + m_size);
  }
}

