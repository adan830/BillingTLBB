#include "billing/net/PacketRoutes.hpp"

#include "billing/database/models/Account.hpp"
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

    // StartUpKick
    m_routers["A9"] = [this](const std::string&)->ResponseData
    {
      return this->onPingConnectionHandle();
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

    std::string responseHexStr;
    responseHexStr.append(m_checkSumFirstStr);

    responseHexStr.append(m_checkSumLastStr);
    return Utils::hexToBytes(responseHexStr);
  }

  PacketRoutes::ResponseData
  PacketRoutes::onLoginRequestHandle(const std::string& packetHexStr)
  {
    std::cout << __FUNCTION__ << std::endl;

    std::string responseHexStr;
    responseHexStr.append(m_checkSumFirstStr);

    // MacAddress start

    // MacAddress end

    // AccountName start
    std::size_t accountNameOffset = 14;
    std::size_t accountNameSize = std::stoul(
      packetHexStr.substr(accountNameOffset, 2), nullptr, 16
      );
    std::cout << "Account name size: " << accountNameSize
    << " - Hex: "
    << packetHexStr.substr(accountNameOffset + 2 , accountNameSize * 2)
    << std::endl;
    auto accountNameBytes = Utils::hexToBytes(
      packetHexStr.substr(accountNameOffset + 2, accountNameSize * 2)
      );
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
    std::cout << "Account name: " << accountName << std::endl;
    // AccountName end

    // Password start
    std::size_t passwordOffset = accountNameOffset + 2 + accountNameSize * 2;
    std::size_t passwordSize = std::stoul(
      packetHexStr.substr(passwordOffset, 2), nullptr, 16
      );
    std::cout << "Password size: " << passwordSize
    << " - Hex: " << packetHexStr.substr(passwordOffset, 2)
    << std::endl;
    auto passwordBytes = Utils::hexToBytes(
      packetHexStr.substr(passwordOffset + 2, passwordSize * 2)
      );
    auto password = std::string(
      passwordBytes.cbegin(),
      passwordBytes.cend()
      );
    std::cout << "Password: " << password << std::endl;
    // Password end

    responseHexStr.append(m_checkSumLastStr);
    return Utils::hexToBytes(responseHexStr);
  }
}

