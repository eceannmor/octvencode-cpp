#include "io.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>

namespace otbv {

static constexpr char[] SIGNATURE = "OTBV\x96";

inline bool endswith(const std::string &str, const std::string &suffix) {
  if (str.length() >= suffix.length()) {
    return (0 == str.compare(str.length() - suffix.length(), suffix.length(),
                             suffix));
  } else {
    return false;
  }
}

void save(std::ostream &stream, const std::vector<bool> &data,
          const std::size_t resolution) {
  /* Should be removed later */
  // if (!endswith(path, ".octv")) {
  //   throw std::invalid_argument(
  //       "Path must end in a file with a .octv extension");
  // }

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
  // same here, resolution should be stored separatelly
  uint32_t meta_res_x = resolution;
  
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

  printf("%zu\n", data.size());
  printf("%d\n", pad_len);
  printf("%zu\n", data_out.size());

  // signature
  stream << SIGNATURE;
  // metadata
  stream << meta_first << meta_res_x << meta_res_y << meta_res_z << meta_data_len;
  // data
  for (std::size_t i = 0; i < data_out.size(); i += 8) {
    uint8_t c = 0;
    for (int j = 0; j < 8; j++) {
      c |= static_cast<char>(data_out[i + j]) << (7 - j);
    }
    stream << c;
  }
}

} // namespace otbv
