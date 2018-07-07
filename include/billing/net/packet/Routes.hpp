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
      // A0
      ByteArray onBLRetConnectHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A1
      ByteArray onBLRetKeepLiveHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A2
      ByteArray onBLRetAccCheckHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A3
      ByteArray onBLRetBillingStartHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A4
      ByteArray onBLRetBillingEndHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A6
      ByteArray onBLRetBillingKeepHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A9
      ByteArray onBLRetKickALLHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      ByteArray onLBLCostLogHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // C4
      ByteArray onBLRetPrizeHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // E1
      ByteArray onBLRetAskBuyHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // E2
      ByteArray onBLRetAskPointHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // F1
      ByteArray onBLRetRegPassPortFactoryHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // 61
      ByteArray onBLRetAuthHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

#if defined(__BILLING_ENTERPRISE_EDITION__)
    private:
      bool isMaxAccountPerMac(
        const std::string& packetHexStr,
        const std::string& currentAccountName
        ) const;
      bool isMaxAccountPerIp(
        const std::string& packetHexStr,
        const std::string& currentAccountName
        ) const;
      bool isMaxAccountPerHardware(
        const std::string& packetHexStr,
        const std::string& currentAccountName
        ) const;
#endif
  };
} }

#endif

