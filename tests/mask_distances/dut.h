#ifndef DUT_H__
#define DUT_H__

#include "util.h"
#include "gravnetconv.h"

const int N = 8;
const int PAR = 1;
const int II = 1;

typedef ap_fixed<16,4> T;

void dut(hls::stream<array<T,N>> inStream[PAR],
         hls::stream<array<T,N>> outStream[PAR],
         hls::stream<int> &numStream);

#endif