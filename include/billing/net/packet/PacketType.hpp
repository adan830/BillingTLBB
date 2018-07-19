#ifndef __BILLING_NET_PACKET_HEX_DATA_HPP__
#define __BILLING_NET_PACKET_HEX_DATA_HPP__

namespace billing { namespace packet {
  enum class PacketType
  {
    LBAskRegLicense = 0x11,
    LBAskAuth = 0x17,
    LBAuth = 0x61,

    LBConnect = 0xA0,
    LBKeppLive,
    LBAccCheck,
    LBBillingStart,
    LBBillingEnd,
    LBBillingKeep = 0xA6,
    LBLRegNewLicense,
    LBKickALL = 0xA9,

    LBLOperateAcc = 0xB1,

    LBLAskNewUserCard = 0xC1,
    LBAskRegAuth,
    LBLNewPrize,
    LBPrize,
    LBLCostLog,
    LBLCardCheckPrize,

    LBAskBuy = 0xE1,
    LBAskPoint,
    LBLNewCheckPrize = 0xE4,
    LBLAskMig,

    LBRegPassPort = 0xF1,
  };
} }

#endif

