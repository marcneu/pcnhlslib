/* Author: Marc Neu - Simplified version of hls4ml latency dense layer. */
#ifndef DENSE_RELU_H__
#define DENSE_RELU_H__

#include <array>
#include "util.h"
#include "macro.h"
#include "hls_stream.h"

using std::array;

template <typename input_t,
          typename output_t,
          typename weight_t,
          typename bias_t,
          typename accum_t,
          int F_IN,
          int F_OUT,
          int II>
void dense_relu(hls::stream<array<input_t,F_IN>> &inputStream,
                hls::stream<array<output_t,F_OUT>> &outputStream,
                weight_t weights[F_IN * F_OUT],
                bias_t biases[F_OUT]) {
    #pragma HLS ARRAY_PARTITION variable=biases complete
    accum_t mult[F_IN * F_OUT];
    #pragma HLS ARRAY_PARTITION variable=mult complete
    accum_t acc[F_OUT];
    #pragma HLS ARRAY_PARTITION variable=acc complete

    HLS_FOR(int ii = 0; ii < II; ii++) {

        array<input_t,F_IN> input;
        inputStream >> input;

        for (int ii = 0; ii < F_IN; ii++) {
            for (int jj = 0; jj < F_OUT; jj++) {
                int index = ii * F_OUT + jj;
                mult[index] = input[ii] * weights[index];
            }
        }

        for (int iacc = 0; iacc < F_OUT; iacc++) {
            acc[iacc] = static_cast<accum_t>(biases[iacc]);
            }

        for (int ii = 0; ii < F_IN; ii++) {
            for (int jj = 0; jj < F_OUT; jj++) {
                int index = ii * F_OUT + jj;
                acc[jj] += mult[index];
            }
        }

        array<output_t,F_OUT> result;

        for (int i = 0; i < F_OUT; i++) {
            if(acc[i] >= static_cast<accum_t>(ap_fixed_max<output_t>())) {
                result[i] = static_cast<output_t>(ap_fixed_max<output_t>());
            } else if(acc[i] > 0)
                result[i] = static_cast<output_t>(acc[i]);
            else
                result[i] = static_cast<output_t>(0.0);        
        }

        outputStream << result;
    }
}

#endif