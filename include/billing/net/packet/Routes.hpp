#ifndef __BILLING_NET_PACKET_ROUTES_HPP__
#define __BILLING_NET_PACKET_ROUTES_HPP__

#include "../Packet.hpp"
#include "../../BaseType.hpp"

#include <unordered_map>
#include <functional>
#include <string>

namespace net { namespace packet {
  class Routes
  {
    public:
      using Routers = std::unordered_map<std::string,
            std::function<ByteArray(const std::shared_ptr<packet::HexData>)>
            >;

    private:
      Routers m_routers;

    private:
      Routes();

    public:
      ~Routes();

    public:
      static Routes& getInstance();
      ByteArray operator[](const std::shared_ptr<Packet> packet);

    protected:
      ByteArray onConnectHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onKeepLiveHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onLoginRequestHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onSelectCharHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onCharLogOutHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onWLBillingKeepHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onAskPrizeAskPointHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onAskPrizeAskBuyHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;
      ByteArray onStartUpKickHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

#if defined(__BILLING_ENTERPRISE_EDITION__)
    private:
      bool isMaxAccountPerMac(
        const std::string& packetHexStr
        ) const;
      bool isMaxAccountPerIp(
        const std::string& packetHexStr
        ) const;
      bool isMaxAccountPerHardware(
        const std::string& packetHexStr
        ) const;
#endif
  };
} }

#endif

