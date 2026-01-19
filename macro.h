#ifndef MACRO_H__
#define MACRO_H__

/**
 * @brief Conditional for-loop macro that compiles to a for-loop or nothing based on HLS mode.
 * 
 * This macro creates a for loop during simulation and expands to nothing during synthesis.
 * Wark around is required when using free running loops in Vitis HLS, due to how the control
 * logic is generated.
 */
#ifndef __SYNTHESIS__ 
    #define HLS_FOR(...) for(__VA_ARGS__)
#else
    #define HLS_FOR(...) for(__VA_ARGS__) _Pragma("HLS PIPELINE II=1 style=flp")
#endif

#endif