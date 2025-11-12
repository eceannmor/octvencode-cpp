# libotbv
Library for reading/writing `.otbv` files. 

OTBV is a file format for compressed binary volumes. <br/>
See formal specifications at [eceannmor.com/OTBV_specification.html](https://eceannmor.com/OTBV_specification.html).

To load a given file, use `otbv::load`. The volume is automatically decompressed and reshaped.
```cpp
using volume = std::vector<std::vector<std::vector<bool>>>;

std::string filename = "test_volume.otbv";
volume 3d_volume_data = otbv::load(filename);
```

To save a 3d volume to a file, use `otbv::save`. The metadata about the volume is stored within the same file.
```cpp
using volume = std::vector<std::vector<std::vector<bool>>>;

std::string filename = "test_volume.otbv";
volume data = create_3d_volume();
otbv::save(filename, data);
```

If you are using flattened volumes, you can pass a 1d data vector and the desired resolution. The volume will be automatically reshaped.
```cpp
std::string filename = "test_volume.otbv";
std::vector<bool> data = create_flattened_3d_volume();
std::tuple<int, int, int> resolution = {32, 32, 64};
otbv::save(filename, data, resolution);
```



See also [otbv-python](https://github.com/eceannmor/otbv-python)

