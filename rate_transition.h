#ifndef __RATE_TRANSITION_H__
#define __RATE_TRANSITION_H__
#include "macro.h"

template<typename T,
         int PAR_IN,
         int PAR_OUT,
         int II,
         int MODE> // 0 = UP, 1 = DOWN
void rate_transition(hls::stream<T> inp[PAR_IN],
                     hls::stream<T> outp[PAR_OUT]) {

    static_assert(II % PAR_IN == 0, "II must be divisible by PAR_IN");
    static_assert(II % PAR_OUT == 0, "II must be divisible by PAR_OUT");

    const int PAR = (MODE == 0) ? PAR_IN : PAR_OUT;

    for (int ii = 0; ii < II; ii++) {
        #pragma HLS PIPELINE II=1 style=flp rewind
        for (int p = 0; p < PAR; p++) {
            #pragma HLS UNROLL
            T buf;
            if (MODE == 0) {
                inp[p] >> buf;
                outp[(ii * PAR_IN + p) % PAR_OUT] << buf;
            } else {
                inp[(ii * PAR_OUT + p) % PAR_IN] >> buf;
                outp[p] << buf;
            }
        }
    }
}

#endif // __RATE_TRANSITION_H__