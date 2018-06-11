#ifndef __BILLING_NET_PACKET_BUFFER_CONTROLLER_HPP__
#define __BILLING_NET_PACKET_BUFFER_CONTROLLER_HPP__

#include "../Packet.hpp"

namespace net { namespace packet {
  class BufferController
  {
    private:
      const Packet::Buffer& m_data;

    public:
      BufferController(const Packet::Buffer& buffer);
      ~BufferController();
  };
} }

#endif

