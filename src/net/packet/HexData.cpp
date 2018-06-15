#include "billing/net/packet/HexData.hpp"

#include "billing/Utils.hpp"

namespace net { namespace packet
{
  HexData::HexData()
  {
    this->init();
  }

  HexData::HexData(
    const std::shared_ptr<Packet::Buffer> buffer, const std::size_t size
    ) :
    m_size(size)
  {
    this->init();
  }

  HexData::~HexData()
  {
  }

  void HexData::init()
  {
    m_header = "AA55";
    m_footer = "55AA";
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
} }

