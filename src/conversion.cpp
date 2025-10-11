#include "conversion.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace octvencode {

template <typename T> volume<T> reshape_to_cubic(const std::vector<T> &data) {
  const std::size_t data_size = data.size();
  double edge_len = std::cbrt(data_size);
  if (std::floor(edge_len) != edge_len) {
    throw std::invalid_argument("Could not reshape data into a cubic tensor");
  } else {
    std::size_t new_edge_len = static_cast<std::size_t>(edge_len);
    volume<T> out;
    out.resize(new_edge_len);
    for (auto row : out) {
      row.resize(new_edge_len);
      for (auto col : row) {
        col.resize(new_edge_len);
      }
    }
    std::size_t id = 0;
    // trying to match numpy's default reshape() order. have not tested yet.
    // *must* be consistent between this and the python library
    for (std::size_t x = 0; x < edge_len; ++x) {
      for (std::size_t y = 0; y < edge_len; ++y) {
        for (std::size_t z = 0; z < edge_len; ++z) {
          out[x][y][z] = data[id++];
        }
      }
    }
    return out;
  }
}

template <typename T> bool is_volume_homogeneous(const volume<T> &data) {
  return false; //: C
}

template <typename T> inline unsigned long long size(const volume<T> &data) {
  return data.size() * data[0].size() * data[0][0].size();
}
} // namespace octvencode
