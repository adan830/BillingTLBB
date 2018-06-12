#include "billing/Utils.hpp"

namespace Utils
{
  std::vector<char> hexToBytes(const std::string& hex)
  {
    std::vector<char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
      std::string byteString = hex.substr(i, 2);
      char byte = (char) strtol(byteString.c_str(), NULL, 16);
      bytes.emplace_back(byte);
    }

    return bytes;
  }

  std::string strToHex(const char* data, const std::size_t len)
  {
    static constexpr char hexmap[] = {
      '0', '1', '2', '3', '4', '5', '6', '7',
      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    static std::string hexStr(len * 2, ' ');
    for (std::size_t i = 0; i < len; ++i)
    {
      hexStr[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
      hexStr[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return hexStr;
  }
}

