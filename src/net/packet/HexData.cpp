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

      m_size = std::stoul(hexData.substr(4, 4), nullptr, 16);

      m_header = hexData.substr(0, 4);

      m_type = hexData.substr(8, 2);

      std::size_t bodyLastPos = m_size * 2 - 2;
      if (bodyLastPos > (hexData.size() - 12))
      {
        throw std::runtime_error("Error size");
      }

      m_body = hexData.substr(10, bodyLastPos);

      m_footer = hexData.substr(hexData.size() - 4);
    }
    catch(const std::exception& e)
    {
      LOG->error("Error while parse buffer: {}", e.what());
    }

    this->init();
  }

  HexData::~HexData()
  {
  }

  void HexData::init()
  {
    if (
      m_footer.empty() || m_header.empty() ||
      (m_header != "AA55") || (m_footer != "55AA")
      )
    {
      LOG->error("Buffer error");
      delete this;
      return;
    }
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

