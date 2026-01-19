#ifndef __STACK_H__
#define __STACK_H__

template<typename T,
         int W,
         int PAR,
         int II>
void stack(hls::stream<array<T, W>> inputStream[PAR],
           hls::stream<array<T, W*II*PAR>> &outputStream) {
    array<T, W*II*PAR> output;
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind
        for(int p = 0; p < PAR; p++) {
            array<T, W> input;
            inputStream[p] >> input;
            for(int w = 0; w < W; w++) {
                output[ii*PAR*W + p*W + w] = input[w];
            }
            if(ii == II - 1 && p == PAR - 1)
                outputStream << output;
        }
    }
}

#endif