#include "billing/net/packet/BufferController.hpp"

#include "billing/Utils.hpp"

#include <iostream>

namespace net { namespace packet {
  BufferController::BufferController(const Packet::Buffer& buffer) :
    m_data(buffer)
  {
    this->dataHandle();
  }

  BufferController::~BufferController()
  {
  }

  const std::vector<char>& BufferController::getResponseData() const
  {
    std::cout << "Responsing..." << std::endl;
    return m_responseData;
  }

  void BufferController::dataHandle()
  {
    this->onOpenConnectionHandle();
  }

  void BufferController::onOpenConnectionHandle()
  {
    m_responseData = Utils::hexToBytes(
#include "../../../deps/res/packets/openConnection.bin"
                                      );
  }
} }

