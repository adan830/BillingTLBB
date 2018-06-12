#ifndef __BILLING_NET_PACKET_ROUTES_HPP__
#define __BILLING_NET_PACKET_ROUTES_HPP__

#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

namespace net
{
  class PacketRoutes
  {
    public:
      using ResponseData = std::vector<char>;
      using Routers = std::unordered_map<std::string, std::function<ResponseData()>>;

    private:
      Routers m_routers;

    private:
      PacketRoutes();

    public:
      ~PacketRoutes();

    public:
      static PacketRoutes& getInstance();
      const ResponseData operator[](const std::string& hexStr);

    protected:
      void dataHandle();
      ResponseData onOpenConnectionHandle();
  };
}

#endif

