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
      // std::this_thread::sleep_for(std::chrono::milliseconds(10000));
      return this->onFirstConnectionHandle();
    };

    m_routers["AA550011A1"] = [this]()->ResponseData
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
    std::cout << __FUNCTION__ << std::endl;
    return Utils::hexToBytes("AA550005A01198010055AA");
    // return Utils::hexToBytes("AA550005A1A8D8060055AA");
  }

  PacketRoutes::ResponseData PacketRoutes::onOpenConnectionHandle()
  {
    std::cout << __FUNCTION__ << std::endl;
    // return ResponseData();
    return Utils::hexToBytes("AA550005A1A8D8060055AA");
    // return Utils::hexToBytes("AA550005A01198010055AA");
  }
}

