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
        throw std::runtime_error("Buffer size is invalid");
      }

      m_header = hexData.substr(0, 4);

      if (m_header != "AA55")
      {
        throw std::runtime_error("Buffer header is invalid");
      }

      auto sizeHex = hexData.substr(m_header.size(), 4);
      m_size = Utils::hexToNumber<HexData::SizeType>(sizeHex);

      m_type = hexData.substr(m_header.size() + sizeHex.size(), 2);

      std::size_t bodyLastPos = (m_size * 2) - m_type.size();
      if (bodyLastPos > (hexData.size() - 12))
      {
        throw std::runtime_error("Buffer size is too large");
      }

      m_id = hexData.substr(
        m_header.size() + sizeHex.size() + m_type.size(), 4
        );
      if (m_id.empty())
      {
        throw std::runtime_error("Not found packet ID");
      }

      m_body = hexData.substr(
        m_header.size() + sizeHex.size() + m_type.size() + m_id.size(),
        bodyLastPos
        );

      if (m_body.empty())
      {
        throw std::runtime_error("Buffer body is empty");
      }

      m_footer = hexData.substr(hexData.size() - 4);

      if (m_footer != "55AA")
      {
        throw std::runtime_error("Buffer footer is empty");
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
  }

  void HexData::init()
  {
  }

  const HexData& HexData::append(const std::string& hexBody)
  {
    m_body.append(hexBody);

    this->updateSize();

    return *this;
  }

  void HexData::setType(const std::string& type)
  {
    m_type = type;

    this->updateSize();
  }

  const std::string& HexData::getType() const
  {
    return m_type;
  }

  const std::string& HexData::getId() const
  {
    return m_id;
  }

  void HexData::setId(const std::string& id)
  {
    m_id = id;

    this->updateSize();
  }

  const std::string& HexData::getBody() const
  {
    return m_body;
  }

  HexData::SizeType HexData::getSize() const
  {
    return m_size;
  }

  std::string HexData::toString() const
  {
    LOG->warning(
      "Header: {} | Size: {} | Type: {} | Id: {} | Body: {} | Footer: {}",
      m_header, m_size, m_type, m_id, m_body, m_footer
      );

    return m_header + Utils::numberToHex(m_size, 4) + m_type + m_id + m_body
    + m_footer;
  }

  ByteArray HexData::toByteArray() const
  {
    return Utils::hexToBytes(this->toString());
  }

  void HexData::updateSize()
  {
    m_size = (m_type.size() + m_id.size() + m_body.size()) / 2;
  }
} }

