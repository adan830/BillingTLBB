#ifndef __BILLING_NET_PACKET_ROUTES_HPP__
#define __BILLING_NET_PACKET_ROUTES_HPP__

#include "../Packet.hpp"
#include "../../BaseType.hpp"

#include <unordered_map>
#include <functional>
#include <string>

namespace net { namespace packet {
  /**
   * Note: Decompile Login (IDA Hex-Ray)
   * Function=meant: LB=Login->Billing, BL=Billing->Login
   * Login sent to Billing: LB
   * Billing send to Login: BLRet
   */
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
      // 11
      ByteArray onLBAskRegLicenseHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // 17
      ByteArray onLBAskAuthHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // 61
      ByteArray onLBAuthHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A0
      ByteArray onLBConnectHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A1
      ByteArray onLBKeepLiveHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A2
      ByteArray onLBAccCheckHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A3
      ByteArray onLBBillingStartHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A4
      ByteArray onLBBillingEndHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A6
      ByteArray onLBBillingKeepHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A7
      ByteArray onLBLRegNewLicenseHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // A9
      ByteArray onLBKickALLHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // B1
      ByteArray onLBLOperateAccHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // C1
      ByteArray onLBLAskNewUserCardHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // C2
      ByteArray onLBAskRegAuthHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // C3
      ByteArray onLBLNewPrizeHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // C4
      ByteArray onLBPrizeHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // C5
      ByteArray onLBLCostLogHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // C6
      ByteArray onLBLCardCheckPrizeHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // E1
      ByteArray onLBAskBuyHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // E2
      ByteArray onLBAskPointHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // E4
      ByteArray onLBLNewCheckPrizeHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // E5
      ByteArray onLBLAskMigHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

      // F1
      ByteArray onLBRegPassPortHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

#if defined(__BILLING_ENTERPRISE_EDITION__)
    protected:
      // FF
      ByteArray onSystemHandle(
        const std::shared_ptr<packet::HexData> hexData
        ) const;

    private:
#  if defined(__BILLING_WITH_ANTI_CLONE_MAC__)
      bool isMaxAccountPerMac(
        const std::string& packetHexStr,
        const std::string& currentAccountName
        ) const;
#  endif
#  if defined(__BILLING_WITH_ANTI_CLONE_IP__)
      bool isMaxAccountPerIp(
        const std::string& packetHexStr,
        const std::string& currentAccountName
        ) const;
#  endif
#  if defined(__BILLING_WITH_ANTI_CLONE_HARDWARE__)
      bool isMaxAccountPerHardware(
        const std::string& packetHexStr,
        const std::string& currentAccountName
        ) const;
#  endif
#endif
  };
} }

#endif

