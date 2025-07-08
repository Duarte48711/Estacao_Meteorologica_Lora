
#include "DataHandling.h"
#include <cstring>
#include <cstdint>
#include <cstddef>

uint8_t* serializeFloatArray(const float* floatArray, size_t count) {
    size_t totalBytes = count * sizeof(float);
    uint8_t* byteArray = new uint8_t[totalBytes];

    for (size_t i = 0; i < count; ++i) {
        memcpy(&byteArray[i * sizeof(float)], &floatArray[i], sizeof(float));
    }

    return byteArray; // Remember to delete[] when you're done
}

float* deserializeFloatArray(const uint8_t* byteArray, size_t count) {
    float* floatArray = new float[count];

    for (size_t i = 0; i < count; ++i) {
        memcpy(&floatArray[i], &byteArray[i * sizeof(float)], sizeof(float));
    }

    return floatArray;
}
