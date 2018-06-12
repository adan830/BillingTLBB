#ifndef __BILLING_UTILS_HPP__
#define __BILLING_UTILS_HPP__

#include <vector>
#include <string>

namespace Utils
{
  std::vector<char> hexToBytes(const std::string& hex);

  std::string bytesToHex(const char* data, const std::size_t len);
}

#endif

