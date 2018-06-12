#include "billing/net/PacketRoutes.hpp"

#include "billing/Utils.hpp"

#include <iostream>

namespace net {
  PacketRoutes::PacketRoutes(const std::string& data) :
    m_data(data)
  {
    this->dataHandle();
  }

  PacketRoutes::~PacketRoutes()
  {
  }

  const std::vector<char>& PacketRoutes::getResponseData() const
  {
    std::cout << "Responsing..." << std::endl;
    return m_responseData;
  }

  void PacketRoutes::dataHandle()
  {
    this->onOpenConnectionHandle();
  }

  void PacketRoutes::onOpenConnectionHandle()
  {
    m_responseData = Utils::hexToBytes("AA550005A01198010055AA");
  }
}

