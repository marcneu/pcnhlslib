#ifndef __TOP_K_BITONIC_SORT_H__
#define __TOP_K_BITONIC_SORT_H__

#include "util.h"
#include "macro.h"
#include "hls_stream.h"
#include "top_k_bitonic_merger.h"
#include "bitonic_sort.h"

using namespace std;

/**
 * @brief Sorts top K elements of an array of length N fully pipelined (II=1).
 * @tparam D Type of distance elements
 * @tparam P type of payload elements
 * @tparam N Number of elements in the array that is to be sorted
 * @tparam K Number of output elements
 * @tparam II Number of iterations
 */
template <class D, class P, int N, int K, int II>
void top_k_bitonic_sort_tree(hls::stream<array<D, K>> keysInStream[N / K],
                             hls::stream<array<P, K>> valuesInStream[N / K],
                             hls::stream<array<D, K>> &keysOutStream, 
                             hls::stream<array<P, K>> &valuesOutStream)
{
    #pragma HLS inline
    const int S = ceillog2(N/K);

    hls::stream<array<D, K>> keysStream[S][N / K];
    hls::stream<array<P, K>> valuesStream[S][N / K];

    for (int s = 0; s < S + 1; s++)
    {
        #pragma HLS unroll
        if (s == 0)
        {
            for (int n = 0; n < N / K; n++)
            {
                #pragma HLS unroll
                bitonic_sort<D, P, K, II>(keysInStream[n], valuesInStream[n], keysStream[s][n], valuesStream[s][n]);
            }
        }
        else if (s < S)
        {
            for (int n = 0; n < N / K / static_pow2<int>(s); n++)
            {
                #pragma HLS unroll
                top_k_bitonic_merge<D, P, 2 * K, K, II>(keysStream[s-1][2 * n], valuesStream[s-1][2 * n],
                                                    keysStream[s-1][2 * n + 1], valuesStream[s-1][2 * n + 1],
                                                    keysStream[s][n], valuesStream[s][n]);
            }
        }
        else
        {
            top_k_bitonic_merge<D, P, 2 * K, K, II>(keysStream[s-1][0], valuesStream[s-1][0],
                                                keysStream[s-1][1], valuesStream[s-1][1],
                                                keysOutStream, valuesOutStream);
        }
    }
}

/**
 * @brief Sorts an array of length N of (key,value) tuples and returns top K elements.
 * @param keysInStream Stream interface of length N containing the keys of the first array
 * @param valuesInStream Stream interface of length N containing the values of the first array
 * @param keysOutStream Stream interface of length K containing the sorted key array
 * @param valuesOutStream Stream interface of length K containing the sorted value array
 * @tparam D key type
 * @tparam P value type
 * @tparam N Length of incoming array
 * @tparam K Number of top elements to return
 * @tparam II Number of iterations
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N, int K, int II>
void top_k_bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                        hls::stream<array<P, N>> &valuesInStream,
                        hls::stream<array<D, K>> &keysOutStream,
                        hls::stream<array<P, K>> &valuesOutStream)
{
    HLS_FOR(int ii = 0; ii < II; ii++) {
        array<D, N> keys;
        array<P, N> values;
        array<D, K> topKeys;
        array<P, K> topValues;

        keysInStream >> keys;
        valuesInStream >> values;

        for (int p = 1; p < N; p *= 2)
            for (int k = p; k > 0; k /= 2)
            #pragma HLS dependence variable = keys type = inter false
            #pragma HLS dependence variable = values type = inter false
                for (int j = (k % p); j <= N - 1 - k; j += 2 * k)
                    for (int i = 0; i <= std::min<int>(k - 1, N - j - k - 1); i++)
                        if ((i + j) / (2 * p) == (i + j + k) / (2 * p))
                            compare_and_swap<D, P>(keys[i + j + k], values[i + j + k], keys[i + j], values[i + j]);

        for (int i = 0; i < K; i++) {
            #pragma HLS unroll
            topKeys[i] = keys[i];
            topValues[i] = values[i];
        }

        keysOutStream << topKeys;
        valuesOutStream << topValues;
    }
};

/**
 * @brief Sorts an array of length N of (key,value) tuples and returns top K elements.
 * @param keysInStream Stream interface of length N containing the keys of the first array
 * @param valuesInStream Stream interface of length N containing the values of the first array
 * @param keysOutStream Stream interface of length K containing the sorted key array
 * @param valuesOutStream Stream interface of length K containing the sorted value array
 * @tparam D key type
 * @tparam P value type
 * @tparam N Length of incoming array
 * @tparam K Number of top elements to return
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N, int K>
void top_k_bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                        hls::stream<array<P, N>> &valuesInStream,
                        hls::stream<array<D, K>> &keysOutStream,
                        hls::stream<array<P, K>> &valuesOutStream)
{
    #pragma HLS pipeline II=1 style=frp

    array<D, N> keys;
    array<P, N> values;
    array<D, K> topKeys;
    array<P, K> topValues;

    keysInStream >> keys;
    valuesInStream >> values;

    for (int p = 1; p < N; p *= 2)
        for (int k = p; k > 0; k /= 2)
        #pragma HLS dependence variable = keys type = inter false
        #pragma HLS dependence variable = values type = inter false
            for (int j = (k % p); j <= N - 1 - k; j += 2 * k)
                for (int i = 0; i <= std::min<int>(k - 1, N - j - k - 1); i++)
                    if ((i + j) / (2 * p) == (i + j + k) / (2 * p))
                        compare_and_swap<D, P>(keys[i + j + k], values[i + j + k], keys[i + j], values[i + j]);

    for (int i = 0; i < K; i++) {
        #pragma HLS unroll
        topKeys[i] = keys[i];
        topValues[i] = values[i];
    }

    keysOutStream << topKeys;
    valuesOutStream << topValues;
};

/**
 * @brief Sorts an array of length N of (key,value) tuples and returns top K values only.
 * @param keysInStream Stream interface of length N containing the keys of the first array
 * @param valuesInStream Stream interface of length N containing the values of the first array
 * @param valuesOutStream Stream interface of length K containing the sorted value array
 * @tparam D key type
 * @tparam P value type
 * @tparam N Length of incoming array
 * @tparam K Number of top elements to return
 * @tparam II Number of iterations
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N, int K, int II>
void top_k_bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                        hls::stream<array<P, N>> &valuesInStream,
                        hls::stream<array<P, K>> &valuesOutStream)
{
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind

        array<D, N> keys;
        array<P, N> values;
        array<P, K> topValues;

        keysInStream >> keys;
        valuesInStream >> values;

        for (int p = 1; p < N; p *= 2)
            for (int k = p; k > 0; k /= 2)
            #pragma HLS dependence variable = keys type = inter false
            #pragma HLS dependence variable = values type = inter false
                for (int j = (k % p); j <= N - 1 - k; j += 2 * k)
                    for (int i = 0; i <= std::min<int>(k - 1, N - j - k - 1); i++)
                        if ((i + j) / (2 * p) == (i + j + k) / (2 * p))
                            compare_and_swap<D, P>(keys[i + j + k], values[i + j + k], keys[i + j], values[i + j]);

        for (int i = 0; i < K; i++) {
            #pragma HLS unroll
            topValues[i] = values[i];
        }

        valuesOutStream << topValues;
    }
};

/**
 * @brief Sorts an array of length N of (key,value) tuples and returns top K values only.
 * @param keysInStream Stream interface of length N containing the keys of the first array
 * @param valuesInStream Stream interface of length N containing the values of the first array
 * @param valuesOutStream Stream interface of length K containing the sorted value array
 * @tparam D key type
 * @tparam P value type
 * @tparam N Length of incoming array
 * @tparam K Number of top elements to return
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N, int K>
void top_k_bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                        hls::stream<array<P, N>> &valuesInStream,
                        hls::stream<array<P, K>> &valuesOutStream)
{
    #pragma HLS pipeline II=1 style=frp

    array<D, N> keys;
    array<P, N> values;
    array<P, K> topValues;

    keysInStream >> keys;
    valuesInStream >> values;

    for (int p = 1; p < N; p *= 2)
        for (int k = p; k > 0; k /= 2)
        #pragma HLS dependence variable = keys type = inter false
        #pragma HLS dependence variable = values type = inter false
            for (int j = (k % p); j <= N - 1 - k; j += 2 * k)
                for (int i = 0; i <= std::min<int>(k - 1, N - j - k - 1); i++)
                    if ((i + j) / (2 * p) == (i + j + k) / (2 * p))
                        compare_and_swap<D, P>(keys[i + j + k], values[i + j + k], keys[i + j], values[i + j]);

    for (int i = 0; i < K; i++) {
        #pragma HLS unroll
        topValues[i] = values[i];
    }

    valuesOutStream << topValues;
};

#endif
