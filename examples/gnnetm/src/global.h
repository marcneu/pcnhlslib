#ifndef GLOBALS_H__
#define GLOBALS_H__

#include "util.h"

/* Input Parameters */
static const int ENERGY_OFFSET = 1;

/* Model Parameters */
static const int N = 32;
static const int MODEL_INPUT_WIDTH = 5;
static const int MODEL_OUTPUT_WIDTH = 9;

static const int DENSE_SCALE_OUTPUT_WIDTH = 5;

static const int DENSE_1_OUTPUT_WIDTH = 16;
static const int DENSE_2_OUTPUT_WIDTH = 6;
static const int DENSE_3_OUTPUT_WIDTH = 8;

static const int GRAVNET_1_OUTPUT_WIDTH = 32;

static const int DENSE_4_OUTPUT_WIDTH = 32;
static const int DENSE_5_OUTPUT_WIDTH = 16;
static const int DENSE_6_OUTPUT_WIDTH = 16;
static const int DENSE_7_OUTPUT_WIDTH = 6;
static const int DENSE_8_OUTPUT_WIDTH = 8;

static const int GRAVNET_2_OUTPUT_WIDTH = 32;

static const int DENSE_9_OUTPUT_WIDTH = 32;
static const int DENSE_10_OUTPUT_WIDTH = 16;
static const int DENSE_11_OUTPUT_WIDTH = 16;

/* HEAD PARAMETERS */
static const int BETA_WIDTH = 1;
static const int SIGNAL_WIDTH = 1;
static const int TPOS_WIDTH = 3;
static const int ENERGY_WIDTH = 1;
static const int CCORDS_WIDTH = 3;

/* GraVNetConv Parameters */
static const int GRAVNET_1_K = 8; 
static const int GRAVNET_2_K = 8; 

/* Hardware Parameters */
static const int II = 16;
static const int PAR = N / II;

/* Types */
typedef ap_fixed<16,4> model_input_t; 
typedef ap_fixed<16,6> model_output_t; 

typedef ap_fixed<8,3>  dense_data_t;
typedef ap_fixed<9,3>  dense_relu_t;
typedef ap_fixed<9,4>  dense_weight_t;
typedef ap_fixed<9,4>  dense_biases_t;
typedef ap_fixed<22,11> dense_accum_t;
typedef ap_fixed<27,10> first_dense_accum_t;

typedef ap_fixed<16,3>  gravnet_data_t;
typedef ap_fixed<16,3>  gravnet_relu_t;
typedef ap_fixed<16,3>  gravnet_coordinate_t;
typedef ap_fixed<16,3>  gravnet_feature_t;
typedef ap_fixed<34,8>  gravnet_accum_t;
typedef ap_fixed<20,7> gravnet_distance_t; 
typedef ap_ufixed<10,1> gravnet_exponential_t;
typedef ap_fixed<16,3>  gravnet_weight_t;
typedef ap_fixed<16,3>  gravnet_biases_t;

typedef ap_fixed<20,12> cps_distance_t;
typedef ap_fixed<16,6>  cps_beta_t;

/* Kernel Quantizations for final output block */
typedef ap_fixed<16,3> beta_weight_t;
typedef ap_fixed<16,3> beta_biases_t;
typedef ap_fixed<16,3> signal_weight_t;
typedef ap_fixed<16,3> signal_biases_t;
typedef ap_fixed<16,5> energy_weight_t;
typedef ap_fixed<16,5> energy_biases_t;
typedef ap_fixed<16,5> tpos_weight_t;
typedef ap_fixed<16,5> tpos_biases_t;
typedef ap_fixed<16,5> ccoords_weight_t;
typedef ap_fixed<16,5> ccoords_biases_t;

typedef ap_uint<ceillog2(N)>     cps_identifier_t;

static const int BETA_OFFSET = 1;  // I hope 1 is energy

/*Exponential Funktion LUT*/
static const int LUT_SIZE = 9;

#endif