#ifndef __MULTIPLY_H__
#define __MULTIPLY_H__

template<typename inputa_t,
         typename inputb_t,
         typename output_t,
         int II,
         int F>
void multiply(hls::stream<array<inputa_t,F>> &inputAStream,
              hls::stream<array<inputb_t,F>> &inputBStream,
              hls::stream<array<output_t,F>> &outputStream) {
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1
            array<inputa_t,F> a;
            inputAStream >> a;
            array<inputb_t,F> b;
            inputBStream >> b;
            array<output_t,F> result;
            for(int f= 0; f < F; f++) {
                #pragma HLS unroll
                auto c = a[f] * b[f];
                result[f] = static_cast<output_t>(c);
            }
            outputStream << result;
    }
}

template<typename inputa_t,
         typename inputb_t,
         typename output_t,
         int PAR,
         int II,
         int F>
void multiply(hls::stream<array<inputa_t,F>> inputAStream[PAR],
              hls::stream<array<inputb_t,F>> inputBStream[PAR],
              hls::stream<array<output_t,F>> outputStream[PAR]) {
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1
        for(int p = 0; p < PAR; p++) {
            #pragma HLS unroll
            array<inputa_t,F> a;
            inputAStream[p] >> a;
            array<inputb_t,F> b;
            inputBStream[p] >> b;
            array<output_t,F> result;
            for(int f= 0; f < F; f++) {
                #pragma HLS unroll
                auto c = a[f] * b[f];
                result[f] = static_cast<output_t>(c);
            }
            outputStream[p] << result;
        }
    }
}

template<typename inputa_t,
         typename inputb_t,
         typename output_t,
         int scale,
         int PAR,
         int II,
         int F>
void multiply_and_scale(hls::stream<array<inputa_t,F>> inputAStream[PAR],
                        hls::stream<array<inputb_t,F>> inputBStream[PAR],
                        hls::stream<array<output_t,F>> outputStream[PAR]) {
    for(int ii = 0; ii < II; ii++) {
        #pragma HLS pipeline II=1
        for(int p = 0; p < PAR; p++) {
            #pragma HLS unroll
            array<inputa_t,F> a;
            inputAStream[p] >> a;
            array<inputb_t,F> b;
            inputBStream[p] >> b;
            array<output_t,F> result;
            for(int f= 0; f < F; f++) {
                #pragma HLS unroll
                auto c = a[f] * b[f] * scale;
                // result[f] = static_cast<output_t>(c);
                if(c >= static_cast<output_t>(ap_fixed_max<output_t::width,output_t::iwidth>()))
                    result[f] = static_cast<output_t>(ap_fixed_max<output_t::width,output_t::iwidth>());
                else if(c <= static_cast<output_t>(-ap_fixed_max<output_t::width,output_t::iwidth>()))
                    result[f] = static_cast<output_t>(-ap_fixed_max<output_t::width,output_t::iwidth>());
                else
                    result[f] = static_cast<output_t>(c);
                }
            outputStream[p] << result;
        }
    }
}

#endif