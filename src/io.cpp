#include "io.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>

namespace otbv {

inline bool endswith(const std::string &str, const std::string &suffix) {
  if (str.length() >= suffix.length()) {
    return (0 == str.compare(str.length() - suffix.length(), suffix.length(),
                             suffix));
  } else {
    return false;
  }
}

void save(std::string path, const std::vector<bool> &data,
          const std::size_t resolution) {
  if (!endswith(path, ".octv")) {
    throw std::invalid_argument(
        "Path must end in a file with a .octv extension");
  }

  std::ofstream out_file(path, std::ios::binary);

  out_file << static_cast<char>(resolution & 0xFF);
  out_file << static_cast<char>((resolution >> 8) & 0xFF);

  std::size_t rem = data.size() % 8;
  uint8_t pad_len = rem == 0 ? 0 : static_cast<uint8_t>(8 - rem);
  out_file << static_cast<char>(pad_len);

  std::vector<bool> data_out;

  data_out.reserve(pad_len + data.size());
  for (int i = 0; i < pad_len; ++i) {
    data_out.push_back(0);
  }

  data_out.insert(data_out.end(), data.begin(), data.end());

  printf("%zu\n", data.size());
  printf("%d\n", pad_len);
  printf("%zu\n", data_out.size());

  for (std::size_t i = 0; i < data_out.size(); i += 8) {
    uint8_t c = 0;
    for (int j = 0; j < 8; j++) {
      c |= static_cast<char>(data_out[i + j]) << (7 - j);
    }
    out_file << c;
  }

  out_file.close();
}

} // namespace otbv
