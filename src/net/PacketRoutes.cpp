#include "billing/net/PacketRoutes.hpp"

#include "billing/Utils.hpp"

#include <iostream>
#include <utility>
#include <thread>

namespace net
{
  PacketRoutes::PacketRoutes()
  {
    m_routers["AA550011A0"] = [this]()->ResponseData
    {
      return this->onOpenConnectionHandle();
    };

    m_routers["AA550009A1"] = [this]()->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["AA550086A2"] = [this]()->ResponseData
    {
      return this->onLoginRequestHandle();
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
      auto pos = hexStr.find(router.first);
      if (pos != std::string::npos)
      {
        m_responseData = router.second();
        break;
      }
    }

    return m_responseData;
  }

  PacketRoutes::ResponseData PacketRoutes::onOpenConnectionHandle()
  {
    std::cout << __FUNCTION__ << std::endl;

    return Utils::hexToBytes("AA5555AA");
  }

  PacketRoutes::ResponseData PacketRoutes::onPingConnectionHandle()
  {
    std::cout << __FUNCTION__ << std::endl;

    return Utils::hexToBytes("AA5555AA");
  }

  PacketRoutes::ResponseData PacketRoutes::onLoginRequestHandle()
  {
    std::cout << __FUNCTION__ << std::endl;

    return ResponseData();
  }
}

