#ifndef __BILLING_NET_COMMUNICATE_PACKET_DATA_HPP__
#define __BILLING_NET_COMMUNICATE_PACKET_DATA_HPP__

#include <string>

namespace net
{
  class CommunicatePacketData
  {
    private:
      static std::string m_header;
      std::string m_size;
      std::string m_type;
      std::string m_body;
      static std::string m_footer;

    public:
      CommunicatePacketData(const std::string& hexDataStr);

    public:
      const CommunicatePacketData& append(const std::string& hexDataStr);
      const std::string& toHexString() const;
  };
}

#endif

