#include "billing/net/packet/BufferController.hpp"

namespace net { namespace packet {
  BufferController::BufferController(const Packet::Buffer& buffer) :
    m_data(buffer)
  {
  }

  BufferController::~BufferController()
  {
  }
} }

