#include "billing/net/packet/HexData.hpp"

#include "billing/Utils.hpp"
#include "billing/Log.hpp"

namespace net { namespace packet
{
  HexData::HexData()
  {
    m_header = "AA55";
    m_size = 0;
    m_footer = "55AA";

    this->init();
  }

  HexData::HexData(
    const std::shared_ptr<Packet::Buffer> buffer, const std::size_t size
    )
  {
    try
    {
      auto hexData = Utils::bytesToHex(buffer->data(), size);

      if (hexData.size() < 14)
      {
        throw std::runtime_error("Parse buffer to hex error");
      }

      m_size = Utils::hexToNumber<HexData::SizeType>(hexData.substr(4, 4));

      m_header = hexData.substr(0, 4);

      if (m_header != "AA55")
      {
        throw std::runtime_error("Buffer header error");
      }

      m_type = hexData.substr(8, 2);

      std::size_t bodyLastPos = m_size * 2 - 2;
      if (bodyLastPos > (hexData.size() - 12))
      {
        throw std::runtime_error("Buffer size too large");
      }

      m_body = hexData.substr(10, bodyLastPos);

      if (m_body.empty())
      {
        throw std::runtime_error("Buffer body error");
      }

      m_footer = hexData.substr(hexData.size() - 4);

      if (m_footer != "55AA")
      {
        throw std::runtime_error("Buffer footer error");
      }
    }
    catch(const std::exception& e)
    {
      LOG->error("Error while parse buffer: {}", e.what());
    }
    catch(...)
    {
      LOG->error("Error exception in: {}", __FUNCTION__);
    }

    this->init();
  }

  HexData::~HexData()
  {
    LOG->warning("{} is called", __FUNCTION__);
  }

  void HexData::init()
  {
  }

  const HexData& HexData::append(const std::string& hexBody)
  {
    m_body.append(hexBody);
    m_size = m_type.size() + m_body.size();

    return *this;
  }

  void HexData::setType(const std::string& type)
  {
    m_type = type;

    if (m_size == 0)
    {
      m_size = m_type.size();
    }
  }

  const std::string& HexData::getType() const
  {
    return m_type;
  }

  const std::string& HexData::getBody() const
  {
    return m_body;
  }

  std::string HexData::toString() const
  {
    LOG->warning(
      "Header: {} | Size: {} | Type: {} | Body: {} | Footer: {}",
      m_header, m_size, m_type, m_body, m_footer
      );

    return m_header
    + Utils::numberToHex(m_size, 4)
    + m_type + m_body
    + m_footer;
  }
} }

