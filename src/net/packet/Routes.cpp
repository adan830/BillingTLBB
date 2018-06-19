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

    m_routers["A4"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onCharLogOutHandle(hexData);
    };

    m_routers["A6"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onWLBillingKeepHandle(hexData);
    };

    m_routers["A9"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onStartUpKickHandle(hexData);
    };

    // m_routers["C1"] = [this](const std::shared_ptr<packet::HexData> hexData)
    // ->ByteArray
    // {
    // };

    // m_routers["C5"] = [this](const std::shared_ptr<packet::HexData> hexData)
    // ->ByteArray
    // {
    // };

    // m_routers["C6"] = [this](const std::shared_ptr<packet::HexData> hexData)
    // ->ByteArray
    // {
    // };

    m_routers["E1"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onAskPrizeAskBuyHandle(hexData);
    };

    m_routers["E2"] = [this](const std::shared_ptr<packet::HexData> hexData)
    ->ByteArray
    {
      return this->onAskPrizeAskPointHandle(hexData);
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

  // A0
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

  // A1
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

  // A2
  ByteArray Routes::onLoginRequestHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    auto &packetHexStr = hexData->getBody();

    // MacAddress start
    std::size_t macAddressOffset = packetHexStr.size() - 140;
    auto maxAddressHex = packetHexStr.substr(macAddressOffset, 68);
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
    auto accountNameSize = Utils::hexToNumber<std::size_t>(accountNameSizeHex);
    LOG->warning(
      "Account name size: {} - Hex: {}",
      accountNameSize,
      accountNameSizeHex
      );
    auto accountNameHex = packetHexStr.substr(
       accountNameSizeHex.size(), accountNameSize * 2
      );
    auto accountNameBytes = Utils::hexToBytes(accountNameHex);
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
    LOG->warning("Account name: {}", accountName);
    // AccountName end

    // Password start
    std::size_t passwordOffset = accountNameSizeHex.size() + accountNameSize * 2;
    auto passwordSize = Utils::hexToNumber<std::size_t>(
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
    auto password = std::string(passwordBytes.cbegin(),passwordBytes.cend());
    LOG->warning("Password: {}", password);
    // Password end

    /**
     * List Status
     *
     * 1: OK
     * 2: Wrong info
     * 3: Wrong info
     * 4: Is online
     * 5: Account is exists (for reg form)
     * 6: Fail from server
     * 7: Is being locked
     * 8: Fields empty found
     */
    int loginStatus = 6;
    try
    {
      database::models::Account a(accountName);

      if (a.getIsOnline())
      {
        a.setIsOnline(false);
        loginStatus = 4;
        LOG->error("Account [{}] is currently logged in", a.getName());
      }
      else if (a.getIsLock())
      {
        loginStatus = 7;
        LOG->error("Account [{}] is being locked", a.getName());
      }
      else if (a.getPassword() != password)
      {
        loginStatus = 2;
        LOG->error("Account [{}] tried login with invalid info", a.getName());
      }
      else
      {
        loginStatus = 1;
        LOG->info("Account [{}] sent login request", accountName);
      }
      if (!a.save())
      {
        loginStatus = 6;
      }
    }
    catch (const std::exception& e)
    {
      LOG->error("Sql database error: ", e.what());
    }
    catch (...)
    {
      LOG->error("Exception: Error code not found");
    }

    int loginValue = 1; // Successed
    if (loginStatus != 1)
    {
      loginValue = 1;
    }

    // LastData start
    packet::HexData responseData;
    std::size_t responseDataSize = 50 - 40 * loginValue + accountNameHex.size();
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append(Utils::numberToHex(loginStatus, 2));
    responseData.append(std::string(
        responseDataSize - (responseData.getSize() * 2), '0'
        ));
    // LastData end

    return responseData.toByteArray();
  }

  // A3
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
    auto accountNameBytes = Utils::hexToBytes(accountNameHex);
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
    LOG->warning("Account name: {}", accountName);

    int status = 0;
    try
    {
      database::models::Account a(accountName);

      if (!a.getIsOnline())
      {
        a.setIsOnline(true);
      }

      if (a.save())
      {
        status = 1;
      }
    }
    catch (const std::exception& e)
    {
      LOG->error("Sql database error: ", e.what());
    }
    catch (...)
    {
      LOG->error("Exception: Error code not found");
    }

    // LastData start
    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append(Utils::numberToHex(status, 2));
    responseData.append("00000027100000270F000022B800");
    // LastData end

    return responseData.toByteArray();
  }

  // A4
  ByteArray Routes::onCharLogOutHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    auto &packetHexStr = hexData->getBody();

    auto accountNameSizeHex = packetHexStr.substr(0, 2);
    auto accountNameSize = Utils::hexToNumber<std::size_t>(accountNameSizeHex);
    auto accountNameHex = packetHexStr.substr(
      accountNameSizeHex.size(),
      accountNameSize * 2
      );
    auto accountNameBytes = Utils::hexToBytes(accountNameHex);
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );

    try
    {
      database::models::Account a(accountName);

      if (a.getIsOnline())
      {
        a.setIsOnline(false);
      }
      else
      {
        LOG->warning("Account [{}] is currently not logged in", accountName);
      }

      a.save();
    }
    catch (const std::exception& e)
    {
      LOG->error("Sql database error: ", e.what());
    }
    catch (...)
    {
      LOG->error("Exception: Error code not found");
    }

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append("00");
    return responseData.toByteArray();
  }

#ifndef __BILLING_ENTERPRISE_EDITION__

  // E1
  ByteArray Routes::onAskPrizeAskBuyHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    return responseData.toByteArray();
  }

#endif

  // E2
  ByteArray Routes::onAskPrizeAskPointHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    auto &packetHexStr = hexData->getBody();

    auto accountNameSizeHex = packetHexStr.substr(0, 2);
    auto accountNameSize = Utils::hexToNumber<std::size_t>(accountNameSizeHex);
    auto accountNameHex = packetHexStr.substr(
      accountNameSizeHex.size(),
      accountNameSize * 2
      );
    auto accountNameBytes = Utils::hexToBytes(accountNameHex);
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
    LOG->warning("Account name: {}", accountName);

    int accountPoint = 0;

    try
    {
      database::models::Account a(accountName);

      accountPoint = a.getPoint();
    }
    catch (const std::exception& e)
    {
      LOG->error("Sql database error: ", e.what());
    }
    catch (...)
    {
      LOG->error("Exception: Error code not found");
    }

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append(Utils::numberToHex(accountPoint, 8));
    return responseData.toByteArray();
  }

  // A6
  ByteArray Routes::onWLBillingKeepHandle(
    const std::shared_ptr<packet::HexData> hexData
    )
  {
    LOG->warning(__FUNCTION__);

    auto &packetHexStr = hexData->getBody();
    auto accountNameSizeHex = packetHexStr.substr(0, 2);
    auto accountNameSize = Utils::hexToNumber<std::size_t>(accountNameSizeHex);
    auto accountNameHex = packetHexStr.substr(
      accountNameSizeHex.size(),
      accountNameSize * 2
      );
    auto accountNameBytes = Utils::hexToBytes(accountNameHex);
    auto accountName = std::string(
      accountNameBytes.cbegin(),
      accountNameBytes.cend()
      );
    LOG->warning("Account name: {}", accountName);

    // TODO: Select Database

    packet::HexData responseData;
    responseData.setType(hexData->getType());
    responseData.setId(hexData->getId());
    responseData.append(accountNameSizeHex);
    responseData.append(accountNameHex);
    responseData.append("010000");
    return responseData.toByteArray();
  }
} }

