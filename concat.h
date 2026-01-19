#ifndef __CONCAT_H__
#define __CONCAT_H__

#include "macro.h"

template<typename T1,
         typename T2,
         typename TO,
         int INPUT_WIDTH_1,
         int INPUT_WIDTH_2,
         int II>
void concat(hls::stream<array<T1, INPUT_WIDTH_1>> &in1Stream,
            hls::stream<array<T2, INPUT_WIDTH_2>> &in2Stream,
            hls::stream<array<TO, INPUT_WIDTH_1+INPUT_WIDTH_2>> &outStream) {
    array<T1, INPUT_WIDTH_1> in1;
    array<T2, INPUT_WIDTH_2> in2;
    array<TO, INPUT_WIDTH_1+INPUT_WIDTH_2> out;

    #pragma HLS array_partition variable=in1 complete dim=1
    #pragma HLS array_partition variable=in2 complete dim=1 
    #pragma HLS array_partition variable=out complete dim=1

    HLS_FOR (int ii = 0; ii < II; ii++) {

        in1Stream >> in1;
        in2Stream >> in2;

        for (int i = 0; i < INPUT_WIDTH_1; i++) {
            #pragma HLS UNROLL
            out[i] = static_cast<TO>(in1[i]);
        }

        for (int i = 0; i < INPUT_WIDTH_2; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + i] =  static_cast<TO>(in2[i]);
        }

        outStream << out;
    }
}

template<typename T1,
         typename T2,
         typename T3,
         typename TO,
         int INPUT_WIDTH_1,
         int INPUT_WIDTH_2,
         int INPUT_WIDTH_3,
         int II>
void concat(hls::stream<array<T1, INPUT_WIDTH_1>> &in1Stream,
            hls::stream<array<T2, INPUT_WIDTH_2>> &in2Stream,
            hls::stream<array<T3, INPUT_WIDTH_3>> &in3Stream,
            hls::stream<array<TO, INPUT_WIDTH_1+INPUT_WIDTH_2+INPUT_WIDTH_3>> &outStream) {
    array<T1, INPUT_WIDTH_1> in1;
    array<T2, INPUT_WIDTH_2> in2;
    array<T3, INPUT_WIDTH_3> in3;
    array<TO, INPUT_WIDTH_1+INPUT_WIDTH_2+INPUT_WIDTH_3> out;

    #pragma HLS array_partition variable=in1 complete dim=1
    #pragma HLS array_partition variable=in2 complete dim=1 
    #pragma HLS array_partition variable=in3 complete dim=1
    #pragma HLS array_partition variable=out complete dim=1

    HLS_FOR (int ii = 0; ii < II; ii++) {

        in1Stream >> in1;
        in2Stream >> in2;
        in3Stream >> in3;

        for (int i = 0; i < INPUT_WIDTH_1; i++) {
            #pragma HLS UNROLL
            out[i] = static_cast<TO>(in1[i]);
        }

        for (int i = 0; i < INPUT_WIDTH_2; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + i] =  static_cast<TO>(in2[i]);
        }

        for (int i = 0; i < INPUT_WIDTH_3; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + INPUT_WIDTH_2 + i] =  static_cast<TO>(in3[i]);
        }

        outStream << out;
    }
}

template<typename T,
         int INPUT_WIDTH_1,
         int INPUT_WIDTH_2,
         int INPUT_WIDTH_3,
         int II>
void concat(hls::stream<array<T, INPUT_WIDTH_1>> &in1Stream,
            hls::stream<array<T, INPUT_WIDTH_2>>  &in2Stream,
            hls::stream<array<T, INPUT_WIDTH_3>>  &in3Stream,
            hls::stream<array<T, INPUT_WIDTH_1+INPUT_WIDTH_2+INPUT_WIDTH_3>> &outStream) {
    array<T, INPUT_WIDTH_1> in1;
    array<T, INPUT_WIDTH_2> in2;
    array<T, INPUT_WIDTH_3> in3;
    array<T, INPUT_WIDTH_1+INPUT_WIDTH_2+INPUT_WIDTH_3> out;

    #pragma HLS array_partition variable=in1 complete dim=1
    #pragma HLS array_partition variable=in2 complete dim=1
    #pragma HLS array_partition variable=in3 complete dim=1
    #pragma HLS array_partition variable=out complete dim=1

    HLS_FOR (int ii = 0; ii < II; ii++) {

        in1Stream >> in1;
        in2Stream >> in2;
        in3Stream >> in3;

        for (int i = 0; i < INPUT_WIDTH_1; i++) {
            #pragma HLS UNROLL
            out[i] = in1[i];
        }

        for (int i = 0; i < INPUT_WIDTH_2; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + i] = in2[i];
        }

        for (int i = 0; i < INPUT_WIDTH_3; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + INPUT_WIDTH_2 + i] = in3[i];
        }

        outStream << out;
    }
}


template<typename T,
         int INPUT_WIDTH_1,
         int INPUT_WIDTH_2,
         int INPUT_WIDTH_3,
         int INPUT_WIDTH_4,
         int INPUT_WIDTH_5,
         int II>
void concat(hls::stream<array<T, INPUT_WIDTH_1>> &in1Stream,
            hls::stream<array<T, INPUT_WIDTH_2>>  &in2Stream,
            hls::stream<array<T, INPUT_WIDTH_3>>  &in3Stream,
            hls::stream<array<T, INPUT_WIDTH_4>>  &in4Stream,
            hls::stream<array<T, INPUT_WIDTH_5>>  &in5Stream,
            hls::stream<array<T, INPUT_WIDTH_1+INPUT_WIDTH_2+INPUT_WIDTH_3+INPUT_WIDTH_4+INPUT_WIDTH_5>> &outStream) {
    array<T, INPUT_WIDTH_1> in1;
    array<T, INPUT_WIDTH_2> in2;
    array<T, INPUT_WIDTH_3> in3;
    array<T, INPUT_WIDTH_4> in4;
    array<T, INPUT_WIDTH_5> in5;
    array<T, INPUT_WIDTH_1+INPUT_WIDTH_2+INPUT_WIDTH_3+INPUT_WIDTH_4+INPUT_WIDTH_5> out;

    #pragma HLS array_partition variable=in1 complete dim=1
    #pragma HLS array_partition variable=in2 complete dim=1
    #pragma HLS array_partition variable=in3 complete dim=1
    #pragma HLS array_partition variable=in4 complete dim=1
    #pragma HLS array_partition variable=in5 complete dim=1
    #pragma HLS array_partition variable=out complete dim=1

    HLS_FOR (int ii = 0; ii < II; ii++) {

        in1Stream >> in1;
        in2Stream >> in2;
        in3Stream >> in3;
        in4Stream >> in4;  
        in5Stream >> in5;

        for (int i = 0; i < INPUT_WIDTH_1; i++) {
            #pragma HLS UNROLL
            out[i] = in1[i];
        }

        for (int i = 0; i < INPUT_WIDTH_2; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + i] = in2[i];
        }

        for (int i = 0; i < INPUT_WIDTH_3; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + INPUT_WIDTH_2 + i] = in3[i];
        }

        for (int i = 0; i < INPUT_WIDTH_4; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + INPUT_WIDTH_2 + INPUT_WIDTH_3 + i] = in4[i];
        }

        for (int i = 0; i < INPUT_WIDTH_5; i++) {
            #pragma HLS UNROLL
            out[INPUT_WIDTH_1 + INPUT_WIDTH_2 + INPUT_WIDTH_3 + INPUT_WIDTH_4 + i] = in5[i];
        }

        outStream << out;
    }
}

#endif
