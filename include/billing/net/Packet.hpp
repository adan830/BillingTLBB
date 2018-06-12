#ifndef __BILLING_NET_PACKET_HPP__
#define __BILLING_NET_PACKET_HPP__

#include <memory>
#include <array>

namespace net
{
  class Packet // : public std::enable_shared_from_this<Packet>
  {
    public:
      using Buffer = std::array<char, 1024>;

    private:
      std::shared_ptr<Buffer> m_buffer;
      std::size_t m_size;

    public:
      Packet() = delete;
      Packet(const std::shared_ptr<Buffer> buffer, const std::size_t size);
      ~Packet();

    public:
      const std::shared_ptr<Buffer> getBuffer() const;
      std::size_t getSize() const;
      void setSize(const std::size_t size);
      std::string& toString() const;
      std::string& toHexString() const;
  };
}

#endif

