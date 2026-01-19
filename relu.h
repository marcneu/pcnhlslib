#ifndef RELU_H__
#define RELU_H__

#include <array>
#include "util.h"
#include "macro.h"
#include "hls_stream.h"

using std::array;

template <typename input_t,
          typename output_t,
          int F,
          int II>
void relu(hls::stream<array<input_t,F>> &inputStream,
          hls::stream<array<output_t,F>> &outputStream) {
    HLS_FOR(int ii = 0; ii < II; ii++) {
        array<input_t,F> input;
        inputStream >> input; 
        array<output_t,F> result;  
        for (int i = 0; i < F; i++) {
             result[i] = (input[i] > 0) ? input[i] : static_cast<output_t>(0);
        }
        outputStream << result;  
    }
}

#endif
