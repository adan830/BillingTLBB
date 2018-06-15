#ifndef __BILLING_NET_PACKET_ROUTES_HPP__
#define __BILLING_NET_PACKET_ROUTES_HPP__

#include "../Packet.hpp"

#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

namespace net { namespace packet
{
  class Routes
  {
    public:
      using ResponseData = std::vector<char>;
      using Routers = std::unordered_map<std::string,
            std::function<ResponseData(const std::shared_ptr<packet::HexData>)>
            >;

    private:
      Routers m_routers;
      const std::string m_checkSumFirstStr;
      const std::string m_checkSumLastStr;

    private:
      Routes();

    public:
      ~Routes();

    public:
      static Routes& getInstance();
      ResponseData operator[](const std::shared_ptr<Packet> packet);

    protected:
      ResponseData onOpenConnectionHandle();
      ResponseData onPingConnectionHandle();
      ResponseData onLoginRequestHandle(
        const std::shared_ptr<packet::HexData> packetHexData
        );
  };
} }

#endif

