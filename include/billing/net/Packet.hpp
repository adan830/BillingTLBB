#ifndef __BILLING_NET_PACKET_HPP__
#define __BILLING_NET_PACKET_HPP__

#include <string>
#include <memory>
#include <array>

namespace net
{
  namespace packet
  {
    class HexData;
  }
  class Session;

  class Packet // : public std::enable_shared_from_this<Packet>
  {
    public:
      using Buffer = std::array<char, 1024>;

    private:
      const std::shared_ptr<Buffer> m_buffer;
      std::string m_string;
      std::string m_hexString;
      std::shared_ptr<packet::HexData> m_hexData;

    public:
      Packet() = delete;
      Packet(const std::shared_ptr<Buffer> buffer, const std::size_t size);
      Packet(std::shared_ptr<packet::HexData> hexData);
      ~Packet();

    public:
      const std::shared_ptr<Buffer> getBuffer() const;
      std::size_t getSize() const;
      const std::string& toString() const;
      std::shared_ptr<packet::HexData> getHexData() const;
  };
}

#endif

