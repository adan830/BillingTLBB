#ifndef __BILLING_NET_PACKET_ROUTES_HPP__
#define __BILLING_NET_PACKET_ROUTES_HPP__

#include <string>
#include <vector>

namespace net {
  class PacketRoutes
  {
    private:
      const std::string& m_data;
      std::vector<char> m_responseData;

    public:
      PacketRoutes(const std::string& buffer);
      ~PacketRoutes();

    public:
      const std::vector<char>& getResponseData() const;

    protected:
      void dataHandle();
      void onOpenConnectionHandle();
  };
}

#endif

