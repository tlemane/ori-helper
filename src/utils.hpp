#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

template<typename T>
void check_fstream_good(const std::string& path, const T& stream)
{
  if (!stream.good())
  {
    if constexpr(std::is_same_v<T, std::ofstream>)
      throw std::runtime_error("Unable to write at " + path);
    else
      throw std::runtime_error("Unable to read at " + path);
  }
}