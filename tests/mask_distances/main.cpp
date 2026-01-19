#include <iostream>
#include "hls_stream.h"
#include "dut.h"

int main()
{
    bool fail = false;

    hls::stream<array<T,N>> inputStream[PAR];
    hls::stream<array<T,N>> outputStream[PAR];
    hls::stream<int> numStream;

    int num = 3;
    numStream << num;

    array<float,N> stimuli = {{2.5, -1.25, 4.5, -3.2, 0.8, -5.1, 6.4, -2.9}};

    array<T,N> input;
    for(int n = 0; n < N; n++) {
        input[n] = stimuli[n];
    }

    array<float,N> golden = {{2.5, -1.25, 4.5, -7.999755859375,-7.999755859375,-7.999755859375,-7.999755859375,-7.999755859375}};

    inputStream[0] << input;

    dut(inputStream, outputStream, numStream);

    for(int p = 0; p < PAR; p++) {
        array<T,N> output;
        outputStream[p] >> output;

        for(int n = 0; n < N; n++) {
            if(((float )output[n]) != golden[n]) {
                fail = true;
                printf("Got %1.15f but expected %1.15f\n",((float) output[n]),golden[n]);
            }
        }
    }

    if(fail) {
        std::cout << "FAIL" << std::endl;
    } else {
        std::cout << "SUCCESS" << std::endl;
    }

    return 0;
}