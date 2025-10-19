#include "conversion.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace otbv {
volume<bool> reshape_to_cubic(const std::vector<bool> &data) {
  const std::size_t data_size = data.size();
  double edge_len = std::cbrt(data_size);
  if (std::floor(edge_len) != edge_len) {
    throw std::invalid_argument("Could not reshape data into a cubic tensor");
  } else {
    std::size_t new_edge_len = static_cast<std::size_t>(edge_len);
    volume<bool> out;
    out.resize(new_edge_len);
    for (auto &row : out) {
      row.resize(new_edge_len);
      for (auto &col : row) {
        col.resize(new_edge_len);
      }
    }
    std::size_t id = 0;
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

// template <typename T> bool is_volume_homogeneous(const volume<T> &data) {
//   return false; // :C
// }

template <typename T>
bool is_subvolume_homogeneous(const volume<T> &data, std::size_t xs,
                              std::size_t xe, std::size_t ys, std::size_t ye,
                              std::size_t zs, std::size_t ze) {
  std::size_t subvolume_size = (xe - xs) * (ye - ys) * (ze - zs);
  if (subvolume_size < 2) {
    return true;
  }
  const T &first = data[xs][ys][zs];
  for (std::size_t x = xs; x < xe; ++x) {
    for (std::size_t y = ys; y < ye; ++y) {
      for (std::size_t z = zs; z < ze; ++z) {
        if (data[x][y][z] != first) {
          return false;
        }
      }
    }
  }
  return true;
}

template <typename T> inline unsigned long long size(const volume<T> &data) {
  return data.size() * data[0].size() * data[0][0].size();
}

void encode_recursive(const volume<bool> &data, std::vector<bool> &encoding,
                      std::size_t xs, std::size_t xe, std::size_t ys,
                      std::size_t ye, std::size_t zs, std::size_t ze) {
  // start index inclusive, end index exclusive
  std::size_t subvolume_size = (xe - xs) * (ye - ys) * (ze - zs);
  if (0 == subvolume_size) {
    printf("%zu, %zu, %zu, %zu, %zu, %zu\n", xs, xe, ys, ye, zs, ze);
    throw std::invalid_argument(
        "Encountered a subvolume with a size of 0 when encoding the volume. "
        "This should never happen. Please open a new issue and attach the data "
        "you are trying to encode. "
        "https://github.com/eceannmor/otbv-cpp/issues");
  }
  if (is_subvolume_homogeneous(data, xs, xe, ys, ye, zs, ze)) {
    // leaf
    encoding.push_back(0);
    encoding.push_back(data[xs][ys][zs]);
    return;
  }

  std::size_t x_split = (xe + xs) >> 1;
  std::size_t y_split = (ye + ys) >> 1;
  std::size_t z_split = (ze + zs) >> 1;

  encoding.push_back(1);

  for (char i = 0; i < 2; i++) {
    std::size_t xfirst = i ? x_split : xs;
    std::size_t xsecond = i ? xe : x_split;
    for (char j = 0; j < 2; j++) {
      std::size_t yfirst = j ? y_split : ys;
      std::size_t ysecond = j ? ye : y_split;
      for (char k = 0; k < 2; k++) {
        std::size_t zfirst = k ? z_split : zs;
        std::size_t zsecond = k ? ze : z_split;
        encode_recursive(data, encoding, xfirst, xsecond, yfirst, ysecond,
                         zfirst, zsecond);
      }
    }
  }
}

/**
 * @brief Encodes the binary volume. By this step, the volume should be
 * converted to bool
 *
 * @return std::vector<bool>
 */
std::vector<bool> encode(const volume<bool> &data) {
  std::vector<bool> out;
  size_t resolution = data.size();
  encode_recursive(data, out, 0, resolution, 0, resolution, 0, resolution);
  return out;
}

} // namespace otbv
