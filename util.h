#ifndef __UTIL_H__
#define __UTIL_H__

#include <array>
#include "hls_math.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"

using std::array;

template <typename T>
static constexpr T ceildiv(T dividend, T divisor)
{
#pragma HLS INLINE
    return (dividend + divisor - 1) / divisor;
}

template <typename T>
static constexpr T roundup(T dividend, T divisor)
{
#pragma HLS INLINE
    return ceildiv(dividend, divisor) * divisor;
}

template <class key_t, class val_t>
void compare_and_swap(key_t &keyA, val_t &valA, key_t &keyB, val_t &valB)
{
    #pragma HLS inline
    if (keyA > keyB)
    {
        key_t t;
        t = keyA;
        keyA = keyB;
        keyB = t;
        val_t s;
        s = valA;
        valA = valB;
        valB = s;
    }
};

template <typename T>
static constexpr T static_pow2(T exponent)
{
    T result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result *= 2;
    }
    return result;
}

template <int WIDTH,
          int IWIDTH>
static constexpr float ap_fixed_max() {
    float max = static_pow2<float>(static_cast<int>(IWIDTH - 1)) - 1.0 + (1.0 - 1.0 / static_cast<float>(static_pow2<int>(WIDTH - IWIDTH)));
    return max;
}

template <typename T>
struct is_signed_ap_fixed : std::false_type {};

template <int W, int I, ap_q_mode Q, ap_o_mode O, int N>
struct is_signed_ap_fixed<ap_fixed<W, I, Q, O, N>> : std::true_type {};

template <typename T>
struct is_unsigned_ap_fixed : std::false_type {};

template <int W, int I, ap_q_mode Q, ap_o_mode O, int N>
struct is_unsigned_ap_fixed<ap_ufixed<W, I, Q, O, N>> : std::true_type {};

template <typename T>
static constexpr float ap_fixed_max() {
    const int IWIDTH = T::iwidth;
    const int WIDTH = T::width;
    const int S = (is_signed_ap_fixed<T>::value) ? 1 : 0;
    float max = static_pow2<float>(static_cast<int>(IWIDTH - S)) - 1.0 + (1.0 - 1.0 / static_cast<float>(static_pow2<int>(WIDTH - IWIDTH)));
    return max;
}

constexpr int ceillog2(int x) {
    int result = 0;
    while (x > 1) {
        x = (x + 1) / 2;
        ++result;
    }
    return result;
}

constexpr int floorlog2(int x) {
    int result = 0;
    while (x >= 2) {
        x /= 2;
        ++result;
    }
    return result;
}

template <typename T, int I>
void repeat(hls::stream<T> &in, hls::stream<T> &out)
{
    T data;
    for (int i = 0; i < I; i++)
    {
        #pragma HLS pipeline II = 1 rewind
        if (i == 0)
        {
            in >> data;
        }
        out << data;
    }
}

template <typename T, int II, int N>
void repeat(hls::stream<T> in[N], hls::stream<T> out[N]) {
    T data[N];
    for (int i = 0; i < II; i++) {
        #pragma HLS pipeline II = 1 rewind
        for(int n = 0; n < N; n++) {
            if (i == 0) {
                in[n] >> data[n];
            }
            out[n] << data[n];
        }
    }
}

template <typename T, int II, int N, int M>
void repeat(hls::stream<T> in[N][M], hls::stream<T> out[N][M]) {
    T data[N][M];
    for (int i = 0; i < II; i++) {
        #pragma HLS pipeline II = 1 rewind
        for(int n = 0; n < N; n++) {
            for(int m = 0; m < M; m++) {
                if (i == 0) {
                    in[n][m] >> data[n][m];
                }
                out[n][m] << data[n][m];
            }
        }
    }
}

template<typename TC>
auto exp2(TC const &c) -> decltype(c*c) {
    #pragma HLS inline
    decltype(c*c) const  res = c*c;
    #pragma HLS BIND_OP variable=res op=mul impl=dsp
    return  res;
}

template <typename T, int N>
static constexpr std::array<T, N> init_array(const T value)
{
    #pragma HLS inline
    std::array<T, N> a{};
    for (auto i = 0; i < a.size(); i++)
        #pragma HLS unroll
        a[i] = value;
    return a;
}

template <typename T, int N>
static constexpr std::array<T, N> init_range(int offset,int step)
{
    #pragma HLS inline
    std::array<T, N> a{};
    for (auto i = 0; i < a.size(); i+=step)
        #pragma HLS unroll
        a[i] = static_cast<T>(i+offset);
    return a;
}

template <typename T, int N, int K, int II>
static void split(hls::stream<array<T, N>> &in, hls::stream<array<T, K>> out[N / K])
{
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II = 1
        array<T, N> dataIn;
        in >> dataIn;

        array<T, K> dataOut[ceildiv<int>(N, K)];

        for (int n = 0; n < N; n++)
        {
            #pragma HLS unroll
            dataOut[n / K][n % K] = dataIn[n];
        }

        for (int i = 0; i < N / K; i++)
        {
            #pragma HLS unroll
            out[i] << dataOut[i];
        }
    }
}

template<int N,
         int II>
void pack(hls::stream<ap_uint<N>> &inputStream,
          hls::stream<ap_axiu<N,0,0,0>> &outputStream) {
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind
        ap_uint<N> input;
        inputStream >> input;
        ap_axiu<N,0,0,0> output;
        output.data = input;
        if(ii == II - 1)
            output.last = 1;
        else
            output.last = 0;
        outputStream << output;
    }
}

template<class T,
         int F,
         int II>
void pack(hls::stream<array<T,F>> &inputStream,
          hls::stream<ap_axiu<T::width*F,0,0,0>> &outputStream,
          hls::stream<bool> &flagStream) {
    const int bytes = (T::width*F + 7)/8;
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind 
        bool flag;
        flagStream >> flag;
        array<T,F> input;
        inputStream >> input;
        ap_axiu<T::width*F,0,0,0> output;
        for(int f = 0; f < F; f++) {
            output.data.range((f+1)*T::width-1, f*T::width) = input[f].range(T::width-1,0);
        }
        output.keep = (flag) ? static_cast<ap_uint<bytes>>( (1 << bytes) - 1) : static_cast<ap_uint<bytes>>(0);
        if(ii == II - 1)
            output.last = 1;
        else
            output.last = 0;
        outputStream << output;
    }
}


template<class T,
         int F,
         int II>
void pack(hls::stream<array<T,F>> &inputStream,
          hls::stream<ap_axiu<T::width*F,0,0,0>> &outputStream) {
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind 
        array<T,F> input;
        inputStream >> input;
        ap_axiu<T::width*F,0,0,0> output;
        for(int f = 0; f < F; f++) {
            output.data.range((f+1)*T::width-1, f*T::width) = input[f].range(T::width-1,0);
        }
        if(ii == II - 1)
            output.last = 1;
        else
            output.last = 0;
        outputStream << output;
    }
}

template<class T,
         int F,
         int II>
void unpack(hls::stream<ap_axiu<T::width*F,0,0,0>> &inputStream,
            hls::stream<array<T,F>> &outputStream) {
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind
        ap_axiu<T::width*F,0,0,0> input;
        inputStream >> input;
        array<T,F> output;
        for(int f = 0; f < F; f++) {
            output[f].range(T::width-1,0) = input.data.range((f+1)*T::width-1, f*T::width);
        }
        outputStream << output;
    }
}

template<class T,
         int F,
         int II>
void unpack(hls::stream<ap_axiu<T::width*F,0,0,0>> &inputStream,
            hls::stream<array<T,F>> &outputStream,
            hls::stream<bool> &flagStream) {
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind
        ap_axiu<T::width*F,0,0,0> input;
        inputStream >> input;

        array<T,F> output;
        for(int f = 0; f < F; f++) {
            output[f].range(T::width-1,0) = input.data.range((f+1)*T::width-1, f*T::width);
        }
        outputStream << output;

        bool flag = (input.keep > 0);
        flagStream << flag;
    }
}

#endif
