/**
Description: Top-level test file for standalone caloclusternet test suite.
Author: Marc Neu
Date: 20.11.2024
*/
#include <iostream>
#include "getopt.h"
#include "caloclusternet.h"

#if !defined(__HLS_COSIM__)
    #include <highfive/highfive.hpp>
#endif

using std::array;

#if !defined(__HLS_COSIM__)
    const int MAX_EVENTS = 1000000;
#else
    const int MAX_EVENTS = 1;
#endif

char* input_path = (char *) "build/test.h5";
char* output_path = (char *) "build/output.h5";
char* intermediate_dir = (char *) "build/intermediate";
unsigned num_events = 100;

void parse_command_line(int argc, char *argv[]) {

    int c;

    while (1)
    {
    static struct option long_options[] =
      {
        /* These options set a flag. */
        {"input_path", required_argument, 0, 'i'},
        {"output_path", required_argument, 0, 'o'},
        {"num_events", required_argument, 0, 'n'},
        {"intermediate_dir", required_argument, 0, 'd'},
        {0, 0, 0, 0}
      };
      /* getopt_long stores the option index here. */
      int option_index = 0;

    c = getopt_long (argc, argv, "i:o:n:d:", long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
            case 'i':
                input_path = (char*) optarg;
                break;

            case 'o':
                output_path = (char*) optarg;
                break;

            case 'n':
                num_events = (atoi(optarg) < MAX_EVENTS) ? atoi(optarg) : MAX_EVENTS;
                break;
            case 'd':
                intermediate_dir = (char*) optarg;
                break;

            default:
                abort();
        }
    }
}

int main(int argc, char *argv[])
{

    parse_command_line(argc,argv);

    #if !defined(__HLS_COSIM__)
        HighFive::File file(input_path, HighFive::File::ReadOnly);
        
        auto num_nodes_dataset = file.getDataSet("mask_graphs");
        auto num_nodes_data = num_nodes_dataset.read<std::vector<int16_t>>();

        unsigned available_events = num_nodes_data.size();
        if(num_events > available_events) {
            std::cout << "You requested to test " << num_events << " but there are only " << available_events << " available inside the provided dataset. Truncating." << std::endl;
            num_events = available_events;
        }

        auto feature_dataset = file.getDataSet("feat_graphs");
        auto input_features_data = feature_dataset.read<std::vector<std::vector<std::vector<int16_t>>>>();
    #else 
        float input_features_data[MAX_EVENTS][N][MODEL_INPUT_WIDTH] = {0};
        int num_nodes_data[MAX_EVENTS] = {31};
    #endif

    vector<vector<vector<double>>> output_data(num_events, vector<vector<double>>(N, vector<double>(MODEL_OUTPUT_WIDTH, 0.0)));

    hls::stream<int> numStream;
    hls::stream<array<model_input_t,MODEL_INPUT_WIDTH>> inputStream[PAR];

    hls::stream<array<model_output_t,MODEL_OUTPUT_WIDTH>> outputStream[PAR];
    hls::stream<bool> lastStream[PAR];

    cps_beta_t betaThreshold = 0;
    betaThreshold(cps_beta_t::width-1,0) = 0x0028;
    cps_distance_t isolationThreshold = 0;
    isolationThreshold(cps_distance_t::width-1,0) = 0x4D;

    printf("Loading Parameters:\nBeta Threshold: %s \nIsolation Threshold: %s \n",betaThreshold.to_string(10).c_str(),isolationThreshold.to_string(10).c_str());

    for(unsigned e = 0; e < num_events; e++) {

        int num_nodes = static_cast<int>(num_nodes_data[e]);
        std::cout << "Checking Event No. " << e << " with " << num_nodes << " nodes." << std::endl;

        numStream << num_nodes;

        for(int n = 0; n < num_nodes; n++) {
            array<model_input_t,MODEL_INPUT_WIDTH> stimuli;
            for(int f = 0; f < MODEL_INPUT_WIDTH; f++) {
                float quant = static_cast<float>(input_features_data[e][n][f]) / pow(2,model_input_t::width-model_input_t::iwidth);
                stimuli[f] = static_cast<model_input_t>(quant);
            }
            inputStream[n % PAR] << stimuli;
        }

        caloclusternet(inputStream,outputStream,lastStream,numStream,betaThreshold,isolationThreshold);

        for(int n = 0; n < N; n++) {
            bool last;
            lastStream[n % PAR] >> last;
            if(n / PAR == II - 1) {
                if(!last) {
                    std::cout << "Received last on port" << n % PAR << " as " << last  << " but expectecd true." << std::endl;
                }
            }
            else {
                if(last) {
                    std::cout << "Received last on port" << n % PAR << " as " << last  << " but expectecd false." << std::endl;
                }
            }
        }

        for(int n = 0; n < num_nodes; n++) {
            array<model_output_t,MODEL_OUTPUT_WIDTH> output;
            outputStream[n % PAR] >> output;
            for(int f = 0; f < MODEL_OUTPUT_WIDTH; f++) {
                output_data[e][n][f] = static_cast<double>(output[f]);
            }
        }

        std::cout << "Done." << std::endl;
    }

    #if !defined(__HLS_COSIM__)
        HighFive::File output_file(output_path, HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);
        vector<size_t> dims{num_events, N, MODEL_OUTPUT_WIDTH};
        HighFive::DataSet output_dataset = output_file.createDataSet<double>("simulation", HighFive::DataSpace(dims));
        output_dataset.write(output_data);
    #endif

    return 0;
}