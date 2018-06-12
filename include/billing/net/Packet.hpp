#ifndef __BILLING_NET_PACKET_HPP__
#define __BILLING_NET_PACKET_HPP__

#include <memory>
#include <array>

namespace net
{
  class Session;

  class Packet // : public std::enable_shared_from_this<Packet>
  {
    public:
      using Buffer = std::array<char, 1024>;

    private:
      const std::shared_ptr<Buffer> m_buffer;
      std::size_t m_size;
      std::string m_string;
      std::string m_hexString;
      std::shared_ptr<Session> m_session;

    public:
      Packet() = delete;
      Packet(const std::shared_ptr<Buffer> buffer, const std::size_t size);
      ~Packet();

    public:
      const std::shared_ptr<Buffer> getBuffer() const;
      std::size_t getSize() const;
      void setSize(const std::size_t size);
      const std::string& toString() const;
      const std::string& toHexString() const;
      void setSession(const std::shared_ptr<Session> session);
      const std::shared_ptr<Session> getSession() const;
  };
}

#endif

