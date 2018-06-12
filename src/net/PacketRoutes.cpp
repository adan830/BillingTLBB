#include "billing/net/PacketRoutes.hpp"

#include "billing/Utils.hpp"

#include <iostream>
#include <utility>
#include <thread>

namespace net
{
  PacketRoutes::PacketRoutes()
  {
    m_routers["0011A0"] = [this](const std::string&)->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["0009A1"] = [this](const std::string&)->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["0086A2"] = [this](const std::string& hexStr)->ResponseData
    {
      return this->onLoginRequestHandle(hexStr);
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

    static const std::string m_checkSumFirstStr = "AA55";
    static const std::string m_checkSumLastStr = "55AA";

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
      if (pos == m_checkSumFirstStr.size())
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
  PacketRoutes::onLoginRequestHandle(const std::string& hexStr)
  {
    std::cout << __FUNCTION__ << std::endl;

    std::cout << hexStr << std::endl;

    return Utils::hexToBytes("AA5555AA");
  }
}

