#ifndef BITONIC_SORT_H__
#define BITONIC_SORT_H__

#include "util.h"
#include "hls_stream.h"

using namespace std;

/**
 * @brief Sorts an array of length N of (key,value) tuples.
 * @param keysInStream Stream interface containing the keys array
 * @param valuesInStream Stream interface containing the values array
 * @param keysOutStream Stream interface containing the sorted keys array
 * @param valuesOutStream Stream interface containing the sorted values array
 * @tparam D Key type
 * @tparam P Value type
 * @tparam N Length of arrays
 * @tparam II Number of iterations
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N,int II>
void bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                  hls::stream<array<P, N>> &valuesInStream,
                  hls::stream<array<D, N>> &keysOutStream,
                  hls::stream<array<P, N>> &valuesOutStream)
{
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind

        array<D, N> keys;
        array<P, N> values;

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

        keysOutStream << keys;
        valuesOutStream << values;
    }
};

/**
 * @brief Sorts an array of length N of (key,value) tuples.
 * @param keysInStream Stream interface containing the keys array
 * @param valuesInStream Stream interface containing the values array
 * @param keysOutStream Stream interface containing the sorted keys array
 * @param valuesOutStream Stream interface containing the sorted values array
 * @tparam D Key type
 * @tparam P Value type
 * @tparam N Length of arrays
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N>
void bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                  hls::stream<array<P, N>> &valuesInStream,
                  hls::stream<array<D, N>> &keysOutStream,
                  hls::stream<array<P, N>> &valuesOutStream)
{
    #pragma HLS pipeline II=1 style=frp

    array<D, N> keys;
    array<P, N> values;

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

    keysOutStream << keys;
    valuesOutStream << values;
};



/**
 * @brief Sorts an array of length N of (key,value) tuples and returns values only.
 * @param keysInStream Stream interface containing the keys array
 * @param valuesInStream Stream interface containing the values array
 * @param valuesOutStream Stream interface containing the sorted values array
 * @tparam D Key type
 * @tparam P Value type
 * @tparam N Length of arrays
 * @tparam II Number of iterations
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N, int II>
void bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                  hls::stream<array<P, N>> &valuesInStream,
                  hls::stream<array<P, N>> &valuesOutStream)
{
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1 rewind

        array<D, N> keys;
        array<P, N> values;

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
                            
        valuesOutStream << values;
    }
};

/**
 * @brief Sorts an array of length N of (key,value) tuples and returns values only.
 * @param keysInStream Stream interface containing the keys array
 * @param valuesInStream Stream interface containing the values array
 * @param valuesOutStream Stream interface containing the sorted values array
 * @tparam D Key type
 * @tparam P Value type
 * @tparam N Length of arrays
 * @details Latency depends on the pipeline depth. Throughput II is always one.
 */
template <class D, class P, int N>
void bitonic_sort(hls::stream<array<D, N>> &keysInStream,
                  hls::stream<array<P, N>> &valuesInStream,
                  hls::stream<array<P, N>> &valuesOutStream)
{
    #pragma HLS pipeline II=1 style=frp

    array<D, N> keys;
    array<P, N> values;

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
                        
    valuesOutStream << values;
};

#endif
