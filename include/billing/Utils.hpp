#ifndef __BILLING_UTILS_HPP__
#define __BILLING_UTILS_HPP__

#include "BaseType.hpp"

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

namespace Utils
{
  std::string strToUpper(const std::string& s);

  ByteArray hexToBytes(const std::string& hex);

  std::string bytesToHex(const char* data, const std::size_t len);

  template<std::size_t N, typename T>
  inline std::string numberToHex(T i)
  {
    std::stringstream sstream;
    sstream << std::setfill('0') << std::setw(N)
    << std::uppercase
    << std::hex << i;

    return sstream.str();
  }

  template<typename T>
  inline T hexToNumber(const std::string& hexStr)
  {
    return std::stoul(hexStr, nullptr, 16);
  }
}

#endif

