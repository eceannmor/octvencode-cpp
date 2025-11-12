#include "otbv.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

int main() {
  const std::string filename = "test_file.otbv";
  std::vector<bool> data = {
      0, 0, 0, 1, 1, 0, 1, 1, 1, //
      1, 1, 1, 1, 1, 1, 1, 1, 0  //
  };
  bool expected_loaded_data[3][3][2] = {{{0, 0}, {0, 1}, {1, 0}},
                                        {{1, 1}, {1, 1}, {1, 1}},
                                        {{1, 1}, {1, 1}, {1, 0}}};
  std::tuple<size_t, size_t, size_t> resolution = {3, 3, 2};
  otbv::save(filename, data, resolution);
  std::vector<std::vector<std::vector<bool>>> loaded_data =
      otbv::load(filename);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 2; k++) {
        assert(expected_loaded_data[i][j][k] == loaded_data[i][j][k]);
      }
    }
  }
}