#ifndef __SPLIT_H__
#define __SPLIT_H__

using std::array;

template<typename TI,
         typename T1,
         typename T2,
         int W1,
         int W2,
         int II>
void split(hls::stream<array<TI, W1+W2>> &a,
           hls::stream<array<T1, W1>> &b,
           hls::stream<array<T2, W2>> &c) {

    for (int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 style=flp rewind
        array<TI, W1+W2> in;
        array<T1, W1> out1;
        array<T2, W2> out2;

        a >> in;
        for (int i = 0; i < W1; i++) {
            #pragma HLS UNROLL
            out1[i] = static_cast<T1>(in[i]);
        }
        for (int i = 0; i < W2; i++) {
            #pragma HLS UNROLL
            out2[i] = static_cast<T2>(in[W1 + i]);
        }
        b << out1;
        c << out2;
    }
}

template<typename TI,
         typename T1,
         typename T2,
         int W0,
         int W1,
         int W2,
         int II>
void split_strip(hls::stream<array<TI, W0>> &a,
           hls::stream<array<T1, W1>> &b,
           hls::stream<array<T2, W2>> &c) {
    for (int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 style=flp rewind

        array<TI, W0> in;
        array<T1, W1> out1;
        array<T2, W2> out2;

        a >> in;
        for (int i = 0; i < W1; i++) {
            #pragma HLS UNROLL
            out1[i] = static_cast<T1>(in[i]);
        }
        for (int i = 0; i < W2; i++) {
            #pragma HLS UNROLL
            out2[i] = static_cast<T2>(in[W0/2 + i]);
        }
        b << out1;
        c << out2;
    }
}

template<typename TI,
         typename T1,
         typename T2,
         int W0,
         int W1,
         int W2,
         int PAR,
         int II>
void split_strip(hls::stream<array<TI, W0>> a[PAR],
           hls::stream<array<T1, W1>> b[PAR],
           hls::stream<array<T2, W2>> c[PAR]) {
    for (int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 style=flp rewind
        for(int p = 0; p < PAR; p++) {
            array<TI, W0> in;
            array<T1, W1> out1;
            array<T2, W2> out2;

            a[p] >> in;
            for (int i = 0; i < W1; i++) {
                #pragma HLS UNROLL
                out1[i] = static_cast<T1>(in[i]);
            }
            for (int i = 0; i < W2; i++) {
                #pragma HLS UNROLL
                out2[i] = static_cast<T2>(in[W0/2 + i]);
            }
            b[p] << out1;
            c[p] << out2;
        }
    }
}

#endif // __SPLIT_H__