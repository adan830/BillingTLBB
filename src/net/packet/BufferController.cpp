#include "billing/net/packet/BufferController.hpp"

#include "billing/Utils.hpp"

namespace net { namespace packet {
  BufferController::BufferController(const Packet::Buffer& buffer) :
    m_data(buffer)
  {
  }

  BufferController::~BufferController()
  {
  }

  const std::vector<char>& BufferController::getResponseData() const
  {
    static std::vector<char> m;
    m = Utils::hexToBytes(
#include "../../../deps/res/packets/openConnection.bin"
                         );
    return m;
    return m_responseData;
  }
} }

