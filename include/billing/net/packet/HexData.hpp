#ifndef __BILLING_NET_PACKET_HEX_DATA_HPP__
#define __BILLING_NET_PACKET_HEX_DATA_HPP__

#include "../Packet.hpp"
#include <string>

namespace net { namespace packet
{
  class HexData
  {
    private:
      static std::string m_header;
      std::string m_size;
      std::string m_type;
      std::string m_body;
      static std::string m_footer;

    public:
      HexData();
      HexData(
        const std::shared_ptr<Packet::Buffer> buffer, const std::size_t size
        );
      HexData(const std::string& hexDataStr);
      ~HexData();

    public:
      const HexData& append(const std::string& hexBodyStr);
      const std::string& toHexString() const;
      void setType(const std::string& type);
  };
} }

#endif

