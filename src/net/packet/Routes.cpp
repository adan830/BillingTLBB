#include "billing/net/packet/Routes.hpp"

#include "billing/database/models/Account.hpp"
#include "billing/net/packet/HexData.hpp"
#include "billing/Utils.hpp"
#include "billing/Log.hpp"

#include <utility>
#include <thread>

namespace net { namespace packet
{
  Routes::Routes()
  {
    m_routers["A0"] = [this](const std::shared_ptr<packet::HexData>) ->ResponseData
    {
      return this->onOpenConnectionHandle();
    };

    m_routers["A1"] = [this](const std::shared_ptr<packet::HexData>)
    ->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    m_routers["A2"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ResponseData
    {
      return this->onLoginRequestHandle(hexData);
    };

    // StartUpKick
    m_routers["A9"] = [this](const std::shared_ptr<packet::HexData>)
    ->ResponseData
    {
      return this->onPingConnectionHandle();
    };

    // Test
    m_routers["FF"] = [](const std::shared_ptr<packet::HexData>)->ResponseData
    {
      try
      {
        database::models::Account a(1);
      }
      catch(...)
      {
        LOG->error("Error");
      }

      return ResponseData();
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

  Routes::ResponseData
  Routes::operator[](const std::shared_ptr<Packet> hexData)
  {
    ResponseData m_responseData;

    for (const auto& router : m_routers)
    {
      if (hexData->getHexData()->getType() == router.first)
      {
        m_responseData = router.second(hexData->getHexData());
        break;
      }
    }

    return m_responseData;
  }

  Routes::ResponseData Routes::onPingConnectionHandle()
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;
    responseData.setType("A1");

    return Utils::hexToBytes(responseData.toString());
  }

  Routes::ResponseData Routes::onOpenConnectionHandle()
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;

    responseData.setType("A0");

    return Utils::hexToBytes(responseData.toString());
  }

  Routes::ResponseData
  Routes::onLoginRequestHandle(const std::shared_ptr<packet::HexData> hexData)
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;

    auto &packetHexStr = hexData->getBody();

    // MacAddress start
    std::size_t macAddressOffset = packetHexStr.size() - 136;
    auto maxAddressHex = packetHexStr.substr(
      macAddressOffset, packetHexStr.size() - 72
      );
    auto macAddressBytes = Utils::hexToBytes(maxAddressHex);
    LOG->warning(
      "MAC Address offset: {} - Hex: {}",
      macAddressOffset,
      maxAddressHex
      );
    auto macAddress = std::string(
      macAddressBytes.cbegin(),
      macAddressBytes.cend()
      );
    LOG->warning("MAC Address: {}", macAddress);
    // MacAddress end

    // AccountName start
    std::size_t accountNameOffset = 0;
    auto accountNameSizeHex = packetHexStr.substr(accountNameOffset, 2);
    std::size_t accountNameSize = Utils::hexToNumber<std::size_t>(
      accountNameSizeHex
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
    std::size_t passwordSize = Utils::hexToNumber<std::size_t>(
      packetHexStr.substr(passwordOffset, 2)
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
    responseData.append(packetHexStr.substr(10, 4));
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append('0' + std::to_string(loginStatus));
    // responseData.append(std::string(
    //     responseDataSize - responseData.size() - m_checkSumLastStr.size(),
    //     '0'
    //     ));
    // LastData end

    responseData.setType("A2");

    LOG->warning("Login packet Hex: {}", responseData.toString());

    return Utils::hexToBytes(responseData.toString());
  }
} }

