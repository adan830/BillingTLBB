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
    m_routers["A0"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onConnectHandle(hexData);
    };

    m_routers["A1"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onKeepLiveHandle(hexData);
    };

    m_routers["A2"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onLoginRequestHandle(hexData);
    };

    m_routers["A3"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onSelectCharHandle(hexData);
    };

    // m_routers["A4"] = [this](const std::shared_ptr<packet::HexData> hexData)
    // ->ByteArray
    // {
    //   return this->onSelectCharHandle(hexData);
    // };

    m_routers["A9"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onStartUpKickHandle(hexData);
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

  ByteArray Routes::operator[](const std::shared_ptr<Packet> hexData)
  {
    ByteArray m_responseData;

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

  ByteArray Routes::onConnectHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;

    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append("0100");

    return responseData.toByteArray();
  }

  ByteArray Routes::onKeepLiveHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append("0600");

    return Utils::hexToBytes(responseData.toString());
  }

  ByteArray Routes::onStartUpKickHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;
    responseData.setType("A1");
    responseData.setId(hexData->getId());
    responseData.append("0600");

    return responseData.toByteArray();
  }

  ByteArray Routes::onLoginRequestHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

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
    auto accountNameSizeHex = packetHexStr.substr(0, 2);
    std::size_t accountNameSize = Utils::hexToNumber<std::size_t>(
      accountNameSizeHex
      );
    LOG->warning(
      "Account name size: {} - Hex: {}",
      accountNameSize,
      accountNameSizeHex
      );
    auto accountNameHex = packetHexStr.substr(
       accountNameSizeHex.size(), accountNameSize * 2
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
    std::size_t passwordOffset = accountNameSizeHex.size() + accountNameSize * 2;
    std::size_t passwordSize = Utils::hexToNumber<std::size_t>(
      packetHexStr.substr(passwordOffset, 2)
      );
    auto passwordHexStr = packetHexStr.substr(
      passwordOffset + 2, passwordSize * 2
      );
    LOG->warning(
      "Password size: {} - Hex: {}",
      passwordSize,
      passwordHexStr
      );
    auto passwordBytes = Utils::hexToBytes(passwordHexStr);
    auto password = std::string(
      passwordBytes.cbegin(),
      passwordBytes.cend()
      );
    LOG->warning("Password: {}", password);
    // Password end

    int loginStatus = 1; // Successed
    int loginValue = 1; // Successed
    try
    {
      database::models::Account a(accountName);
    }
    catch (const std::exception& e)
    {
      LOG->error("Sql database error: ", e.what());
    }
    catch (...)
    {
      LOG->error("Sql database has error");
    }

    LOG->info("Account [{}] sent login request", accountName);

    // LastData start
    packet::HexData responseData;
    std::size_t responseDataSize = 50 - 40 * loginValue + accountNameHex.size();
    LOG->warning("Login packet size: {}", responseDataSize);
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append(Utils::numberToHex(loginStatus, 2));
    responseData.append(std::string(
        responseDataSize - (responseData.getSize() * 2), '0'
        ));
    // LastData end

    {
      auto resHex = responseData.toString();
      LOG->warning("Login packet Hex: {}:{}", resHex, resHex.size());
    }

    return responseData.toByteArray();
  }

  ByteArray Routes::onSelectCharHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    auto &packetHexStr = hexData->getBody();

    auto accountNameSizeHex = packetHexStr.substr(0, 2);
    auto accountNameSize = Utils::hexToNumber<std::size_t>(accountNameSizeHex);

    auto accountNameHex = packetHexStr.substr(
      accountNameSizeHex.size(), accountNameSize * 2
      );
    auto accountName = Utils::hexToBytes(accountNameHex);

    // LastData start
    packet::HexData responseData;
    int status = 0;
    status = 1;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append(Utils::numberToHex(status, 2));
    responseData.append("00000027100000270F000022B800");
    // LastData end

    return responseData.toByteArray();
  }
} }

