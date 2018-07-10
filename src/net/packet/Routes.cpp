#include "billing/net/packet/Routes.hpp"

#include "billing/database/models/Account.hpp"
#include "billing/net/packet/HexData.hpp"
#include "billing/Utils.hpp"
#include "billing/Log.hpp"

#include <utility>
#include <thread>

namespace net { namespace packet {
  Routes::Routes()
  {
    m_routers["A0"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBConnectHandle(hexData);
    };

    m_routers["A1"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBKeepLiveHandle(hexData);
    };

    m_routers["A2"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBAccCheckHandle(hexData);
    };

    m_routers["A3"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBBillingStartHandle(hexData);
    };

    m_routers["A4"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBBillingEndHandle(hexData);
    };

    m_routers["A6"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBBillingKeepHandle(hexData);
    };

    m_routers["A9"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBKickALLHandle(hexData);
    };

    // TODO: Open this
#if defined(BILLING_DEBUG)
    m_routers["C5"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBLCostLogHandle(hexData);
    };
#endif

#if defined(__BILLING_ENTERPRISE_EDITION__) && defined(__BILLING_WITH_E1__)
    m_routers["E1"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBAskBuyHandle(hexData);
    };
#endif

    m_routers["E2"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLBAskPointHandle(hexData);
    };

#if defined(__BILLING_ENTERPRISE_EDITION__)
    m_routers["FF"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onSystemHandle(hexData);
    };
#endif
  }

  Routes::~Routes()
  {
  }

  Routes& Routes::getInstance()
  {
    static Routes s_instance;
    return s_instance;
  }

  ByteArray Routes::operator[](const std::shared_ptr<Packet> packet)
  {
    ByteArray m_responseData;

    auto hexData = packet->getHexData();
    auto routerIt = std::find_if(
      m_routers.cbegin(), m_routers.cend(),
      [&hexData](const Routers::value_type& it){
        return hexData->getType() == it.first;
      }
      );
    if (routerIt != m_routers.cend())
    {
      try
      {
        m_responseData = routerIt->second(hexData);
      }
      catch(const std::exception& e)
      {
        LOG->error("Solving {}: {}", hexData->getType(), e.what());
      }
      catch(...)
      {
        LOG->error("{}: exeption throwed", __FUNCTION__);
      }
    }

    return m_responseData;
  }

  // A0
  ByteArray Routes::onLBConnectHandle(
    const std::shared_ptr<packet::HexData> hexData
    ) const
  {
#if defined(BILLING_DEBUG)
    LOG->warning(__FUNCTION__);
#endif

    packet::HexData responseData;

    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append("0055");

    return responseData.toByteArray();
  }

  // A1
  ByteArray Routes::onLBKeepLiveHandle(
    const std::shared_ptr<packet::HexData> hexData
    ) const
  {
#if defined(BILLING_DEBUG)
    LOG->warning(__FUNCTION__);
#endif

    auto &packetHexStr = hexData->getBody();
    // ZoneId: 2u
    // WorldId: 2u
    // PlayerCount: 2u

    auto zoneIdHex = packetHexStr.substr(0, 4);
    auto worldIdHex = packetHexStr.substr(zoneIdHex.size(), 4);
    auto playerCountHex = packetHexStr.substr(
      zoneIdHex.size() + worldIdHex.size(), 4
      );
    LOG->info(
      "ZoneId={}, WorldId={}, PlayerCount={}",
      Utils::hexToNumber<unsigned short>(zoneIdHex),
      Utils::hexToNumber<unsigned short>(worldIdHex),
      Utils::hexToNumber<unsigned short>(playerCountHex)
      );

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append("0055");
    return responseData.toByteArray();
  }

  ByteArray Routes::onLBKickALLHandle(
    const std::shared_ptr<packet::HexData> hexData
    ) const
  {
#if defined(BILLING_DEBUG)
    LOG->warning(__FUNCTION__);
#endif

    // ZoneId: 2u
    // WorldId: 2u
    // ReasonLength: 1u
    // ExtInfo1Length 1u
    // ExtInfo2Length: 1u
    // ExtInfo3Length: 1u

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append("00");

    return responseData.toByteArray();
  }

#if defined(__BILLING_ENTERPRISE_EDITION__)
  // A2
#endif

#if !defined(__BILLING_ENTERPRISE_EDITION__)

  // E1
  ByteArray Routes::onAskPrizeAskBuyHandle(
    const std::shared_ptr<packet::HexData> hexData
    ) const
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    return responseData.toByteArray();
  }

#endif

  // E2
  ByteArray Routes::onLBAskPointHandle(
    const std::shared_ptr<packet::HexData> hexData
    ) const
  {
#if defined(BILLING_DEBUG)
    LOG->warning(__FUNCTION__);
#endif

    auto &packetHexStr = hexData->getBody();

    auto accountNameSizeHex = packetHexStr.substr(0, 2);
    auto accountNameSize = Utils::hexToNumber<std::size_t>(accountNameSizeHex);
    auto accountNameHex = packetHexStr.substr(
      accountNameSizeHex.size(),
      accountNameSize * 2
      );
    auto accountNameBytes = Utils::hexToBytes(accountNameHex);
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
#if defined(BILLING_DEBUG)
    LOG->warning("Account name: {}", accountName);
#endif

    int leftPoint = 0;

    try
    {
      database::models::Account a(accountName);

      leftPoint = a.getPoint() * 1000;

      constexpr int maxLeftPoint = 2000000;

      leftPoint = (leftPoint > maxLeftPoint) ? maxLeftPoint : leftPoint;
    }
    catch (const std::exception& e)
    {
      LOG->error("Sql database error: ", e.what());
    }
    catch (...)
    {
      LOG->error("{}: throwed exeption", __FUNCTION__);
    }

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex)
    .append(accountNameHex)
    .append(Utils::numberToHex(leftPoint, 8));
    return responseData.toByteArray();
  }

  // A6
  ByteArray Routes::onLBBillingKeepHandle(
    const std::shared_ptr<packet::HexData> hexData
    ) const
  {
#if defined(BILLING_DEBUG)
    LOG->warning(__FUNCTION__);
#endif

    auto &packetHexStr = hexData->getBody();
    auto accountNameSizeHex = packetHexStr.substr(0, 2);
    auto accountNameSize = Utils::hexToNumber<std::size_t>(accountNameSizeHex);
    auto accountNameHex = packetHexStr.substr(
      accountNameSizeHex.size(),
      accountNameSize * 2
      );
    auto accountNameBytes = Utils::hexToBytes(accountNameHex);
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
    std::size_t charLevelHexOffset = (accountNameSizeHex.size() +
                                      accountNameHex.size());
    auto charLevelHex = packetHexStr.substr(charLevelHexOffset, 4);
    auto timeHex = packetHexStr.substr(
      charLevelHexOffset + charLevelHex.size()
      );
#if defined(BILLING_DEBUG)
    LOG->warning(
      "Account name: {}, Char Level: {}, TimeHex: {}",
      accountName,
      Utils::hexToNumber<unsigned short>(charLevelHex),
      timeHex
      );
#endif

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex)
    .append(accountNameHex)
    .append("01000000000000000000000000");
    return responseData.toByteArray();
  }

  // C5
#if defined(BILLING_DEBUG)
  ByteArray Routes::onLBLCostLogHandle(
    const std::shared_ptr<packet::HexData> hexData
    ) const
  {
#if defined(BILLING_DEBUG)
    LOG->warning(__FUNCTION__);
#endif

    // TODO: Handle this

    return packet::HexData().toByteArray();
  }
#endif
} }

