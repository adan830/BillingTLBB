#ifndef __BILLING_NET_PACKET_BUFFER_CONTROLLER_HPP__
#define __BILLING_NET_PACKET_BUFFER_CONTROLLER_HPP__

#include "../Packet.hpp"
#include <vector>

namespace net { namespace packet {
  class BufferController
  {
    private:
      const Packet::Buffer& m_data;
      std::vector<char> m_responseData;

    public:
      BufferController(const Packet::Buffer& buffer);
      ~BufferController();

    public:
      const std::vector<char>& getResponseData() const;

    protected:
      void dataHandle();
      void onOpenConnectionHandle();
  };
} }

#endif

