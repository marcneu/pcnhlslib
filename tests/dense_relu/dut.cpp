#include "dut.h"

void dut(hls::stream<array<input_relu_t, INPUT_WIDTH>> &inputStream,
         hls::stream<array<output_relu_t, OUTPUT_WIDTH>> &outputStream) {
    #pragma HLS dataflow
    dense<input_relu_t,
            output_relu_t,
            weight_t,
            biases_t,
            accum_t,
            INPUT_WIDTH,
            OUTPUT_WIDTH,
            II>(inputStream,
                outputStream,
                weights,
                biases);
}