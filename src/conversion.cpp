#include "conversion.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

// https://www.reddit.com/r/cpp_questions/comments/1h3sva9/
// not handling edge cases
size_t pow2_roof(size_t number) {
  if (!(number & (number - 1)))
    return number;
  size_t shift = 1;
  size_t val = 0;
  do {
    val = number;
    number |= (number >> shift);
    shift *= 2;
  } while (number > val);
  return val + 1;
}

namespace otbv {

template <typename T> vector3<T> reshape_to_cubic(const std::vector<T> &data) {
  const size_t data_size = data.size();
  double edge_len = std::cbrt(data_size);
  if (std::floor(edge_len) != edge_len) {
    throw std::invalid_argument("Could not reshape data into a cubic tensor");
  } else {
    size_t new_edge_len = static_cast<size_t>(edge_len);
    vector3<T> out;
    out.resize(new_edge_len);
    for (auto &row : out) {
      row.resize(new_edge_len);
      for (auto &col : row) {
        col.resize(new_edge_len);
      }
    }
    size_t id = 0;
    for (size_t x = 0; x < edge_len; ++x) {
      for (size_t y = 0; y < edge_len; ++y) {
        for (size_t z = 0; z < edge_len; ++z) {
          out[x][y][z] = data[id++];
        }
      }
    }
    return out;
  }
}

template <typename T>
vector3<T> reshape(const std::vector<T> &data,
                   const std::tuple<size_t, size_t, size_t> &resolution) {
  const size_t data_size = data.size(), //
      x_res = std::get<0>(resolution),  //
      y_res = std::get<1>(resolution),  //
      z_res = std::get<2>(resolution),  //
      resolution_size = x_res * y_res * z_res;

  if (data_size != resolution_size) {
    throw std::invalid_argument(
        "Provided vector cannot be reshaped to a provided resolution");
  }

  vector3<T> out;
  out.resize(x_res);
  for (auto &row : out) {
    row.resize(y_res);
    for (auto &col : row) {
      col.resize(z_res);
    }
  }
  size_t id = 0;
  for (size_t x = 0; x < x_res; ++x) {
    for (size_t y = 0; y < y_res; ++y) {
      for (size_t z = 0; z < z_res; ++z) {
        out[x][y][z] = data[id++];
      }
    }
  }
  return out;
}

template <typename T>
bool is_subvolume_homogeneous(const vector3<T> &data, size_t xs, size_t xe,
                              size_t ys, size_t ye, size_t zs, size_t ze) {
  size_t subvolume_size = (xe - xs) * (ye - ys) * (ze - zs);
  if (subvolume_size < 2) {
    return true;
  }
  const T &first = data[xs][ys][zs];
  for (size_t x = xs; x < xe; ++x) {
    for (size_t y = ys; y < ye; ++y) {
      for (size_t z = zs; z < ze; ++z) {
        if (data[x][y][z] != first) {
          return false;
        }
      }
    }
  }
  return true;
}

template <typename T> inline size_t size(const vector3<T> &data) {
  return data.size() * data[0].size() * data[0][0].size();
}

size_t size(const vector3<bool> &data) {
  return data.size() * data[0].size() * data[0][0].size();
}

void encode_recursive(const vector3<bool> &data, std::vector<bool> &encoding,
                      size_t xs, size_t xe, size_t ys, size_t ye, size_t zs,
                      size_t ze) {
  // start index inclusive, end index exclusive
  size_t subvolume_size = (xe - xs) * (ye - ys) * (ze - zs);
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

  size_t x_split = (xe + xs) >> 1;
  size_t y_split = (ye + ys) >> 1;
  size_t z_split = (ze + zs) >> 1;

  encoding.push_back(1);

  for (char i = 0; i < 2; i++) {
    size_t xfirst = i ? x_split : xs;
    size_t xsecond = i ? xe : x_split;
    for (char j = 0; j < 2; j++) {
      size_t yfirst = j ? y_split : ys;
      size_t ysecond = j ? ye : y_split;
      for (char k = 0; k < 2; k++) {
        size_t zfirst = k ? z_split : zs;
        size_t zsecond = k ? ze : z_split;
        encode_recursive(data, encoding, xfirst, xsecond, yfirst, ysecond,
                         zfirst, zsecond);
      }
    }
  }
}

std::vector<bool> encode(const vector3<bool> &data) {
  std::vector<bool> out;
  size_t resolution = data.size();
  encode_recursive(data, out, 0, resolution, 0, resolution, 0, resolution);
  return out;
}

void pad_to_cube(vector3<bool> &data) {
  size_t max_res = std::max(data.size(), data[0].size());
  max_res = std::max(max_res, data[0][0].size());
  max_res = pow2_roof(max_res);
  data.resize(max_res);
  for (auto &plane : data) {
    plane.resize(max_res);
    for (auto &col : plane) {
      col.resize(max_res);
    }
  }
}

template <typename T> vector3<T> deep_copy(const vector3<T> &vector) {
  vector3<bool> copy;
  size_t x_res = vector.size(), y_res = vector[0].size(),
         z_res = vector[0][0].size();
  copy.resize(x_res);
  for (auto &row : copy) {
    row.resize(y_res);
    for (auto &col : row) {
      col.resize(z_res);
    }
  }
  size_t id = 0;
  for (size_t x = 0; x < x_res; ++x) {
    for (size_t y = 0; y < y_res; ++y) {
      for (size_t z = 0; z < z_res; ++z) {
        copy[x][y][z] = vector[x][y][z];
      }
    }
  }
  return copy;
}

vector3<bool> pad_to_cube(const vector3<bool> &data) {
  auto copy = deep_copy(data);
  pad_to_cube(copy);
  return copy;
}

} // namespace otbv
