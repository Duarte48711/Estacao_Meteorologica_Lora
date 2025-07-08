
#include <cstdint>
#include <cstddef>


uint8_t* serializeFloatArray(const float* floatArray, size_t count);
float* deserializeFloatArray(const uint8_t* byteArray, size_t count);


