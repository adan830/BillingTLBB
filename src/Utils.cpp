#include "billing/Utils.hpp"

#include "billing/Log.hpp"

namespace Utils
{
  ByteArray hexToBytes(const std::string& hex)
  {
    ByteArray bytes;

    for (std::size_t i = 0; i < hex.size(); i += 2)
    {
      bytes.emplace_back(std::strtol(hex.substr(i, 2).data(), nullptr, 16));
    }

    return bytes;
  }

  std::string bytesToHex(const char* data, const std::size_t len)
  {
    static constexpr char hexmap[] = {
      '0', '1', '2', '3', '4', '5', '6', '7',
      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    std::string hexStr(len * 2, ' ');
    for (std::size_t i = 0; i < len; ++i)
    {
      hexStr[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
      hexStr[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return hexStr;
  }

  std::string strToUpper(const std::string& s)
  {
    std::string str = s;

    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    return str;
  }
}

