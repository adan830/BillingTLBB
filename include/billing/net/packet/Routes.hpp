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
        );
      ByteArray onKeepLiveHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
      ByteArray onLoginRequestHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
      ByteArray onSelectCharHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
      ByteArray onCharLogOutHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
      ByteArray onWLBillingKeepHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
      ByteArray onAskPrizeAskPointHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
      ByteArray onAskPrizeAskBuyHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
      ByteArray onStartUpKickHandle(
        const std::shared_ptr<packet::HexData> hexData
        );
  };
} }

#endif

