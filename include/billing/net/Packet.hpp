#ifndef __BILLING_NET_PACKET_HPP__
#define __BILLING_NET_PACKET_HPP__

#include <array>

namespace net
{
  class Packet
  {
    public:
      using Buffer = std::array<char, 1024>;

    private:
      Buffer m_buffer;

    public:
      Packet();
      Packet(const Buffer& buffer);
      ~Packet();

    public:
      void setBuffer(const Buffer& buffer);
      const Buffer& getBuffer() const;
  };
}

#endif

