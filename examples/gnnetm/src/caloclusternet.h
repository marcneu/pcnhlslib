#ifndef CALOCLUSTERNET_H__
#define CALOCLUSTERNET_H__

#include "caloclusternetv4.h"
#include "weights.h"
#include "global.h"

void caloclusternet(hls::stream<array<model_input_t,  MODEL_INPUT_WIDTH>>  inputStream[PAR],
                    hls::stream<array<model_output_t,  MODEL_OUTPUT_WIDTH>> outputStream[PAR],
                    hls::stream<bool> lastStream[PAR],
                    hls::stream<int> &numStream,
                    cps_beta_t betaThreshold,
                    cps_distance_t isolationThreshold);

#endif