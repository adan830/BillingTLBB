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
    auto hexData = Utils::bytesToHex(buffer->data(), size);

    m_header = hexData.substr(0, 4);

    m_size = std::stoul(hexData.substr(4, 4), nullptr, 16);

    LOG->warning("SIZE: {}", m_size);

    m_type = hexData.substr(8, 2);
    m_body = hexData.substr(10, m_size);

    m_footer = hexData.substr(hexData.size() - 4);
    this->init();
  }

  HexData::~HexData()
  {
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
  }

  std::string HexData::toString() const
  {
    return m_footer
    + Utils::numberToHex(m_size, 4)
    + m_type + m_body
    + m_footer;
  }
} }

