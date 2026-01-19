#ifndef THRESHOLD_H__
#define THRESHOLD_H__

#include <array>
#include "util.h"
#include "macro.h"
#include "hls_stream.h"

using std::array;

template <typename T,
          int F,
          int II>
void threshold(hls::stream<array<T,F>> &input_stream,
               hls::stream<array<T,F>> &output_stream,
               const T &threshold) {
    HLS_FOR(int ii = 0; ii < II; ii++) {
        array<T,F> input;
        input_stream >> input; 
        array<T,F> output;  
        for (int i = 0; i < F; i++) {
            output[i] = (input[i] > threshold) ? static_cast<T>(1) : static_cast<T>(0);
        }
        output_stream << output;  
    }
}

#endif
