#pragma once

#include <vector>

template <typename T> using volume = std::vector<std::vector<std::vector<T>>>;

namespace octvencode {
void encode();
void decode();

/**
 * @brief Returns a copy of \p data reshaped into a cubic tensor, if possible
 *
 * @tparam T Any type
 * @param data A 1d vector, where <code>Ea s.th. a^3 == data.size()</code>
 * @return std::vector<std::vector<std::vector<T>>> Cubic tensor with side
 * length \p a (see \p data)
 */
template <typename T> volume<T> reshape_to_cubic(const std::vector<T> &data);

template <typename T> bool is_volume_homogeneous(const volume<T> &data);

template <typename T> inline unsigned long long size(const volume<T> &data);
} // namespace octvencode
