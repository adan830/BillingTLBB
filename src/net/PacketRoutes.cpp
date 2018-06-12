#include "billing/net/PacketRoutes.hpp"

#include "billing/Utils.hpp"

#include <iostream>
#include <utility>

namespace net
{
  PacketRoutes::PacketRoutes()
  {
    m_routers["AA550011A000"] = [this]()->ResponseData
    {
      return this->onFirstConnectionHandle();
    };

    m_routers["AA550011A031"] = [this]()->ResponseData
    {
      return this->onOpenConnectionHandle();
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
      if (pos == 0)
      {
        m_responseData = router.second();
        break;
      }
    }

    return m_responseData;
  }

  PacketRoutes::ResponseData PacketRoutes::onFirstConnectionHandle()
  {
    return Utils::hexToBytes("AA550005A01198010055AA");
  }

  PacketRoutes::ResponseData PacketRoutes::onOpenConnectionHandle()
  {
    return Utils::hexToBytes("AA550005A01198010055AA");
  }
}

