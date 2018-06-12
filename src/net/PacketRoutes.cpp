#include "billing/net/PacketRoutes.hpp"

#include "billing/Utils.hpp"

#include <iostream>
#include <utility>
#include <thread>

namespace net
{
  PacketRoutes::PacketRoutes() :
    m_checkSumFirstStr("AA55"),
    m_checkSumLastStr("55AA")
  {
    m_routers["A0"] = [this](const std::string&)->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["A1"] = [this](const std::string&)->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["A2"] = [this](const std::string& packetHexStr)->ResponseData
    {
      return this->onLoginRequestHandle(packetHexStr);
    };
  }

  PacketRoutes::~PacketRoutes()
  {
  }

  PacketRoutes& PacketRoutes::getInstance()
  {
    static PacketRoutes s_instance;
    return s_instance;
  }

  const PacketRoutes::ResponseData
  PacketRoutes::operator[](const std::string& hexStr)
  {
    ResponseData m_responseData;

    for (const auto& router : m_routers)
    {
      // TODO: Middle of checksum may not same

      if (hexStr.find(m_checkSumFirstStr) != 0)
      {
        break;
      }
      if (
        hexStr.find(m_checkSumLastStr)
        !=
        (hexStr.size() - m_checkSumLastStr.size())
        )
      {
        break;
      }

      auto pos = hexStr.find(router.first, m_checkSumFirstStr.size());
      if (pos == m_checkSumFirstStr.size() + 4)
      {
        m_responseData = router.second(hexStr);
        break;
      }
    }

    return m_responseData;
  }

  PacketRoutes::ResponseData PacketRoutes::onPingConnectionHandle()
  {
    std::cout << __FUNCTION__ << std::endl;

    return Utils::hexToBytes("AA5555AA");
  }

  PacketRoutes::ResponseData
  PacketRoutes::onLoginRequestHandle(const std::string& packetHexStr)
  {
    std::cout << __FUNCTION__ << std::endl;

    std::string responseHexStr;
    responseHexStr.append(m_checkSumFirstStr);

    // Handle start

    std::size_t accountNameSize = std::atoi(
      Utils::hexToBytes(packetHexStr.substr(15,2)).data()
      );
    std::cout << "Account name size: " << accountNameSize << std::endl;
    auto accountName = packetHexStr.substr(1,2);

    // Handle end

    responseHexStr.append(m_checkSumLastStr);
    return Utils::hexToBytes(responseHexStr);
  }
}

