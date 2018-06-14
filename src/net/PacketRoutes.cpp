#include "billing/net/PacketRoutes.hpp"

#include "billing/database/models/Account.hpp"
#include "billing/Utils.hpp"
#include "billing/Log.hpp"

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
    LOG->warning(__FUNCTION__);

    std::string responseHexStr;
    responseHexStr.append(m_checkSumFirstStr);

    // responseHexStr.append("0000A0");

    responseHexStr.append(m_checkSumLastStr);
    return Utils::hexToBytes(responseHexStr);
  }

  PacketRoutes::ResponseData
  PacketRoutes::onLoginRequestHandle(const std::string& packetHexStr)
  {
    LOG->warning(__FUNCTION__);

    std::string responseHexStr;
    responseHexStr.append(m_checkSumFirstStr);

    // MacAddress start
    std::size_t macAddressOffset = packetHexStr.size() - 140;
    auto macAddressBytes = Utils::hexToBytes(
      packetHexStr.substr(macAddressOffset, packetHexStr.size() - 76)
      );
    LOG->warning(
      "MAC Address offset: {} - Hex: {}",
      macAddressOffset,
      packetHexStr.substr(macAddressOffset, packetHexStr.size() - 76)
      );
    auto macAddress = std::string(
      macAddressBytes.cbegin(),
      macAddressBytes.cend()
      );
    LOG->warning("MAC Address: {}", macAddress);
    // MacAddress end

    // AccountName start
    std::size_t accountNameOffset = 14;
    std::size_t accountNameSize = std::stoul(
      packetHexStr.substr(accountNameOffset, 2), nullptr, 16
      );
    LOG->warning(
      "Account name size: {} - Hex: {}",
      accountNameSize,
      packetHexStr.substr(accountNameOffset + 2 , accountNameSize * 2)
      );
    auto accountNameBytes = Utils::hexToBytes(
      packetHexStr.substr(accountNameOffset + 2, accountNameSize * 2)
      );
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
    LOG->warning("Account name: {}", accountName);
    // AccountName end

    // Password start
    std::size_t passwordOffset = accountNameOffset + 2 + accountNameSize * 2;
    std::size_t passwordSize = std::stoul(
      packetHexStr.substr(passwordOffset, 2), nullptr, 16
      );
    LOG->warning(
      "Password size: {} - Hex: {}",
      passwordSize,
      packetHexStr.substr(passwordOffset + 2 , passwordSize * 2)
      );
    auto passwordBytes = Utils::hexToBytes(
      packetHexStr.substr(passwordOffset + 2, passwordSize * 2)
      );
    auto password = std::string(
      passwordBytes.cbegin(),
      passwordBytes.cend()
      );
    LOG->warning("Password: {}", password);
    // Password end

    LOG->info("Account {} is loggin", accountName);

    // LastData start
    enum class LoginStatus
    {
      ERROR = 0,
      SUCCESS = 1
    };

    auto loginStatus = LoginStatus::SUCCESS;
    std::size_t responseDataSize = 31 - 20 * static_cast<int>(loginStatus)
    + accountName.size();
    responseHexStr.append("0101A2");
    // LastData end

    responseHexStr.append(m_checkSumLastStr);
    return Utils::hexToBytes(responseHexStr);
  }
}

