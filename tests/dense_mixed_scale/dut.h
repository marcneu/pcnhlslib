#ifndef DUT_H__
#define DUT_H__

#include "dense.h"
#include "util.h"
#include <ap_int.h>

// Test parameters for integer dense layer
const int IW = 8;     // Input bit width
const int OW = 16;    // Output bit width
const int AW = 32;    // Accumulator bit width
const int WW = 8;     // Weight bit width
const int F_IN = 4;   // Number of input features
const int F_OUT = 3;  // Number of output features
const int II = 1;     // Iteration interval

void dut(hls::stream<array<ap_int<IW>, F_IN>> &inputStream,
         hls::stream<array<ap_int<OW>, F_OUT>> &outputStream,
         const ap_int<WW> weights[F_IN * F_OUT],
         const ap_int<AW> biases[F_OUT],
         const int scales[F_OUT]);

#endif