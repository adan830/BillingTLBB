#include "billing/net/packet/Routes.hpp"

#include "billing/database/models/Account.hpp"
#include "billing/Utils.hpp"
#include "billing/Log.hpp"

#include <utility>
#include <thread>

namespace net { namespace packet
{
  Routes::Routes() :
    m_checkSumFirstStr("AA55"),
    m_checkSumLastStr("55AA")
  {
    m_routers["A0"] = [this](const std::string&)->ResponseData
    {
      return this->onOpenConnectionHandle();
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

  Routes::~Routes()
  {
  }

  Routes& Routes::getInstance()
  {
    static Routes s_instance;
    return s_instance;
  }

  const Routes::ResponseData
  Routes::operator[](const std::string& hexStr)
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

  Routes::ResponseData Routes::onPingConnectionHandle()
  {
    LOG->warning(__FUNCTION__);

    std::string responseHexStr;
    responseHexStr.append(m_checkSumFirstStr);

    // responseHexStr.append("05A8D8060055");

    responseHexStr.append(m_checkSumLastStr);
    return Utils::hexToBytes(responseHexStr);
  }

  Routes::ResponseData Routes::onOpenConnectionHandle()
  {
    LOG->warning(__FUNCTION__);

    std::string responseHexStr;
    responseHexStr.append(m_checkSumFirstStr);

    // responseHexStr.append("0005A011980100");

    responseHexStr.append(m_checkSumLastStr);
    return Utils::hexToBytes(responseHexStr);
  }

  Routes::ResponseData
  Routes::onLoginRequestHandle(const std::string& packetHexStr)
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
    auto accountNameSizeHex = packetHexStr.substr(accountNameOffset, 2);
    std::size_t accountNameSize = std::stoul(
      accountNameSizeHex, nullptr, 16
      );
    LOG->warning(
      "Account name size: {} - Hex: {}",
      accountNameSize,
      accountNameSizeHex
      );
    auto accountNameHex = packetHexStr.substr(
      accountNameOffset + 2 , accountNameSize * 2
      );
    auto accountNameBytes = Utils::hexToBytes(
      accountNameHex
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
    int loginStatus = 1; // Successed
    int loginValue = 0; // Successed
    std::size_t responseDataSize = 72 - 40 * loginValue + accountNameHex.size();
    LOG->warning("Login packet size: {}", responseDataSize);
    std::size_t packetLen = responseDataSize - m_checkSumFirstStr.size() -
    m_checkSumLastStr.size();
    responseHexStr.append(Utils::numberToHex(packetLen, 4));
    // responseHexStr.append("00");
    responseHexStr.append("A2");
    responseHexStr.append(packetHexStr.substr(10, 4));
    responseHexStr.append(accountNameSizeHex);
    responseHexStr.append(accountNameHex);
    responseHexStr.append('0' + std::to_string(loginStatus));
    responseHexStr.append(std::string(
        responseDataSize - responseHexStr.size() - m_checkSumLastStr.size(),
        '0'
        ));
    // LastData end

    responseHexStr.append(m_checkSumLastStr);

    LOG->warning("Login packet Hex: {}", responseHexStr);

    return Utils::hexToBytes(responseHexStr);
  }
} }

