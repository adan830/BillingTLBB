#include "billing/net/PacketRoutes.hpp"

#include "billing/Utils.hpp"

#include <iostream>
#include <utility>
#include <thread>

namespace net
{
  PacketRoutes::PacketRoutes()
  {
    m_routers["AA550011A0"] = [this](const std::string&)->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["AA550009A1"] = [this](const std::string&)->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["AA550086A2"] = [this](const std::string& hexStr)->ResponseData
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

    for (const auto& router : m_routers)
    {
      // TODO: Check \r\n in some packet first

      auto pos = hexStr.find(router.first);
      if (pos == 0)
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

