#include "io.h"
#include "conversion.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <tuple>

namespace otbv {

static constexpr char SIGNATURE[] = "OTBV\x96";

constexpr bool endswith(const std::string &str, const std::string &suffix) {
  return str.length() >= suffix.length() &&
         0 == str.compare(str.length() - suffix.length(), suffix.length(),
                          suffix);
}

void stream_data_as_file_bytes(
    std::ostream &stream, const std::vector<bool> &data,
    const std::tuple<size_t, size_t, size_t> resolution) {
  /*** metadata ***/
  char rem = data.size() % 8;
  char pad_len = rem == 0 ? 0 : 8 - rem;
  uint8_t meta_first = 0;
  meta_first |= (pad_len << 5);

  // this has to be updated
  // if (volume has been padded){
  //    set this bit to 1
  // }
  if (true) {
    meta_first |= (0 << 4);
  }

  // res
  // same here, resolution should be stored separately
  uint32_t meta_res_x = std::get<0>(resolution);
  // not written if cubic
  uint32_t meta_res_y = 0;
  uint32_t meta_res_z = 0;

  uint32_t meta_data_len = data.size() + pad_len;

  /*** data ***/
  std::vector<bool> data_out;
  data_out.reserve(pad_len + data.size());
  for (int i = 0; i < pad_len; ++i) {
    data_out.push_back(0);
  }
  data_out.insert(data_out.end(), data.begin(), data.end());

  // signature
  stream.write(SIGNATURE, 5);
  // metadata
  stream.write(reinterpret_cast<const char *>(&meta_first), sizeof(meta_first));
  stream.write(reinterpret_cast<const char *>(&meta_res_x), sizeof(meta_res_x));
  stream.write(reinterpret_cast<const char *>(&meta_res_y), sizeof(meta_res_y));
  stream.write(reinterpret_cast<const char *>(&meta_res_z), sizeof(meta_res_z));
  stream.write(reinterpret_cast<const char *>(&meta_data_len),
               sizeof(meta_data_len));
  // data
  for (std::size_t i = 0; i < data_out.size(); i += 8) {
    char c = 0;
    for (int j = 0; j < 8; j++) {
      c |= static_cast<char>(data_out[i + j]) << (7 - j);
    }
    stream.write(&c, 1);
  }
}

void save(std::string filename, std::vector<bool> data) {
  const vector3<bool> data_reshaped = reshape_to_cubic(data);
  save(filename, data_reshaped);
}

void save(std::string filename,
          std::vector<std::vector<std::vector<bool>>> data) {
  size_t x_res, y_res, z_res;
  x_res = data.size();
  if (!x_res) {
    printf("The provided volume size is 0. Nothing will be written");
    return;
  }
  y_res = data[0].size();
  if (!y_res) {
    printf("The provided volume size is 0. Nothing will be written");
    return;
  }
  z_res = data[0][0].size();
  if (!z_res) {
    printf("The provided volume size is 0. Nothing will be written");
    return;
  }
  const std::vector<bool> encoded_data = encode(data);
  auto resolution = std::make_tuple(x_res, y_res, z_res);
  std::ofstream file_out(filename, std::ofstream::binary);
  stream_data_as_file_bytes(file_out, encoded_data, resolution);
  int bytes_written = file_out.tellp();
  if (bytes_written > 0) {
    printf("Written %d bytes", bytes_written);
  }
  file_out.close();
}

} // namespace otbv
