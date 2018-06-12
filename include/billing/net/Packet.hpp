#ifndef __BILLING_NET_PACKET_HPP__
#define __BILLING_NET_PACKET_HPP__

#include <memory>
#include <array>

namespace net
{
  class Packet : public std::enable_shared_from_this<Packet>
  {
    public:
      using Buffer = std::array<char, 1024>;

    private:
      Buffer m_buffer;
      std::size_t m_size;

    public:
      Packet();
      Packet(const std::size_t size);
      Packet(const Buffer& buffer);
      ~Packet();

    public:
      void setBuffer(const Buffer& buffer);
      const Buffer& getBuffer() const;
      Buffer& getBuffer();
      std::size_t getSize() const;
      void setSize(const std::size_t size);
      std::string& toString() const;
  };
}

#endif

