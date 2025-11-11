#pragma once

#include <cstddef>
#include <tuple>
#include <vector>

template <typename T> using vector3 = std::vector<std::vector<std::vector<T>>>;

namespace otbv {

void encode_recursive(const vector3<bool> &data, std::vector<bool> &encoding,
                      const size_t xs, const size_t xe, const size_t ys,
                      const size_t ye, const size_t zs, const size_t ze);

/**
 * @brief Encodes the binary volume. By this step, the volume should be
 * converted to bool
 *
 * @return std::vector<bool>
 */
std::vector<bool> encode(const vector3<bool> &data);

vector3<bool> decode(const std::vector<bool> &encoding,
                     const std::tuple<size_t, size_t, size_t> &resolution);

inline void set_range(vector3<bool>, bool value, const size_t xs,
                      const size_t xe, const size_t ys, const size_t ye,
                      const size_t zs, const size_t ze);

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
bool is_subvolume_homogeneous(const vector3<T> &data, const size_t xs,
                              const size_t xe, const size_t ys, const size_t ye,
                              const size_t zs, const size_t ze);

/**
 * @brief Modifies \par data in-place to pad it to a cube. The resulting cube is
 * the smallest cube with an edge length of a power of 2, that can fit \par
 * data.
 */
void pad_to_cube(vector3<bool> &data);
/**
 * @brief Overload of \ref pad_to_cube when the \par data is const. Returns a
 * copy of data padded to a cube.
 */
vector3<bool> pad_to_cube(const vector3<bool> &data);
/**
 * @brief Returns the smallest power of 2 greater than or equal to \par number.
 */
size_t pow2_roof(size_t number);
/**
 * @brief Returns the smallest power of 2 that is greater or equal to the
 * maximum of dimensions in \par resolution.
 */
size_t max_res_pow2_roof(const std::tuple<size_t, size_t, size_t> &resolution);
/**
 * @brief Returns the smallest power of 2 that is greater or equal to the
 * maximum of \par x_res, \par y_res, and \par z_res.
 */
size_t max_res_pow2_roof(const size_t &x_res, const size_t &y_res,
                         const size_t &z_res);
/**
 * @brief Cuts down the volume to the dimensions specified in \par resolution.
 * Makes the assumption that the dimensions are less or equal to the current
 * size of the volume. No checks are performed.
 */
void cut_volume(vector3<bool> &volume,
                const std::tuple<size_t, size_t, size_t> &resolution);
/**
 * @brief Cuts down the volume to the dimensions specified by \par x_res, \par
 * y_res, and \par z_res. Makes the assumption that the dimensions are less or
 * equal to the current size of the volume. No checks are performed.
 */
void cut_volume(vector3<bool> &volume, const size_t &x_res, const size_t &y_res,
                const size_t &z_res);
} // namespace otbv
