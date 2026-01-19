#include <iostream>
#include "hls_stream.h"
#include "dut.h"

using std::array;

int main()
{
    bool fail = false;

    hls::stream<array<input_relu_t, INPUT_WIDTH>> input_stream;

    array<int, INPUT_WIDTH> stimuli = {2,7,9,0,44,0,0,31,14,2,0,0,49,0,1,29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    for(int ii = 0; ii < II; ii++) {
        array<input_relu_t, INPUT_WIDTH> input;
        for(int f = 0; f < INPUT_WIDTH;f++) {
            input[f].range(input_relu_t::width-1,0) = stimuli[f];
        }
        input_stream << input;
    }

    hls::stream<array<output_relu_t, OUTPUT_WIDTH>> output_stream;

    dut(input_stream, output_stream);

    array<int, OUTPUT_WIDTH> golden = {59,33,42,32,10,14,46,29,18,58,0,72,61,54,21,34,25,34,0,0,0,0,0,73,68,61,11,0,0,2,57,0};

    array<output_relu_t, OUTPUT_WIDTH> output;
    
    for(int ii = 0; ii < II; ii++) {
        output_stream >> output;
    }

    for(int i = 0; i < OUTPUT_WIDTH; i++) {
        int test_value = output[i].range(output_relu_t::width-1,0);
        if(golden[i] != test_value) {
            std::cout << "Expected output[" << i << "]: golden " << golden[i] << ", got " << test_value << std::endl;
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