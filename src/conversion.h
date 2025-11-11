#pragma once

#include <cstddef>
#include <tuple>
#include <vector>

template <typename T> using vector3 = std::vector<std::vector<std::vector<T>>>;

namespace otbv {

void encode_recursive(const vector3<bool> &data, std::vector<bool> &encoding,
                      std::size_t xs, std::size_t xe, std::size_t ys,
                      std::size_t ye, std::size_t zs, std::size_t ze);

/**
 * @brief Encodes the binary volume. By this step, the volume should be
 * converted to bool
 *
 * @return std::vector<bool>
 */
std::vector<bool> encode(const vector3<bool> &data);

template <typename T> vector3<bool> convert_to_bool(const vector3<T> &data);

/**
 * @brief Returns a copy of \p data reshaped into a cubic tensor, if possible
 *
 * @tparam T Any type
 * @param data A 1d vector, where <code>Ea s.th. a^3 == data.size()</code>
 * @return std::vector<std::vector<std::vector<T>>> Cubic tensor with side
 * length \p a (see \p data)
 */
template <typename T> vector3<T> reshape_to_cubic(const std::vector<T> &data);

vector3<bool> reshape(const std::vector<bool> &data,
                   const std::tuple<size_t, size_t, size_t> &resolution);

size_t size(const vector3<bool> &data);

template <typename T>
bool is_subvolume_homogeneous(const vector3<T> &data, std::size_t xs,
                              std::size_t xe, std::size_t ys, std::size_t ye,
                              std::size_t zs, std::size_t ze);

void pad_to_cube(vector3<bool> &data);

vector3<bool> pad_to_cube(const vector3<bool> &data);
} // namespace otbv
