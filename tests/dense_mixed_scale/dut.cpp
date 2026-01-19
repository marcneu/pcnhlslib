#include "dut.h"

void dut(hls::stream<array<ap_int<IW>, F_IN>> &inputStream,
         hls::stream<array<ap_int<OW>, F_OUT>> &outputStream,
         const ap_int<WW> weights[F_IN * F_OUT],
         const ap_int<AW> biases[F_OUT],
         const int scales[F_OUT]) {

    dense<IW, OW, AW, WW, F_IN, F_OUT, II>(
        inputStream,
        outputStream,
        weights,
        biases,
        scales
    );
}