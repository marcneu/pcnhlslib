#include "dut.h"

void dut(hls::stream<array<T,N>> inStream[PAR],
         hls::stream<array<T,N>> outStream[PAR],
         hls::stream<int> &numStream) {
    mask_distances<T,N,PAR,II>(
        inStream,
        outStream,
        numStream);
}