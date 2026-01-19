#include <iostream>
#include "hls_stream.h"
#include "dut.h"

using std::array;

int main()
{
    bool fail = false;

    ap_int<WW> weights[F_IN * F_OUT] = {
        10, 20, 5, 
        -5, 15, 10, 
        8, -10, 12,  
        3, 7, -8 
    };

    ap_int<AW> biases[F_OUT] = {100, -50, 25};

    int scales[F_OUT] = {2, 1, 3};

    hls::stream<array<ap_int<IW>, F_IN>> input_stream;

    array<ap_int<IW>, F_IN> input = {1, 2, -3, 4};

    for(int ii = 0; ii < II; ii++) {
        input_stream << input;
    }

    hls::stream<array<ap_int<OW>, F_OUT>> output_stream;

    dut(input_stream, output_stream, weights, biases, scales);

    array<ap_int<OW>, F_OUT> golden = {22, 29, -3};

    array<ap_int<OW>, F_OUT> output;
    output_stream >> output;

    for(int i = 0; i < F_OUT; i++) {
        if(golden[i] != output[i]) {
            std::cout << "Expected output[" << i << "]: golden " << golden[i] << ", got " << output[i] << std::endl;
            fail = true;
        }
    }

    if(fail) {
        std::cout << "TEST FAILED" << std::endl;
        return 1;
    } else {
        std::cout << "TEST PASSED" << std::endl;
        return 0;
    }
}