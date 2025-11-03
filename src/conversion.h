#pragma once

#include <cstddef>
#include <vector>

template <typename T> using volume = std::vector<std::vector<std::vector<T>>>;

namespace otbv {

void encode_recursive(const volume<bool> &data, std::vector<bool> &encoding,
                      std::size_t xs, std::size_t xe, std::size_t ys,
                      std::size_t ye, std::size_t zs, std::size_t ze);

/**
 * @brief Encodes the binary volume. By this step, the volume should be
 * converted to bool
 *
 * @return std::vector<bool>
 */
std::vector<bool> encode(const volume<bool> &data);

template <typename T> volume<bool> convert_to_bool(const volume<T> &data);

/**
 * @brief Returns a copy of \p data reshaped into a cubic tensor, if possible
 *
 * @tparam T Any type
 * @param data A 1d vector, where <code>Ea s.th. a^3 == data.size()</code>
 * @return std::vector<std::vector<std::vector<T>>> Cubic tensor with side
 * length \p a (see \p data)
 */
template <typename T> volume<T> reshape_to_cubic(const std::vector<T> &data);

template <typename T> inline unsigned long long size(const volume<T> &data);

template <typename T>
bool is_subvolume_homogeneous(const volume<T> &data, std::size_t xs,
                              std::size_t xe, std::size_t ys, std::size_t ye,
                              std::size_t zs, std::size_t ze);

void pad_to_cube(volume<bool> &data);
} // namespace otbv
