#ifndef MONITOR_H__
#define MONITOR_H__

#include <array>
#include "util.h"
#include "hls_stream.h"

#if !defined(__HLS_COSIM__)
    #include <highfive/highfive.hpp>
    #include <vector>
    #include <fstream>
    #include <string>
#endif

using std::array;

template<typename T,
         int F,
         int PAR,
         int II>
void monitor(hls::stream<array<T,F>> inputStream[PAR],
             hls::stream<array<T,F>> outputStream[PAR],
             std::string name) {
    std::vector<std::vector<std::vector<int32_t>>> slice(1, vector<vector<int32_t>>(II*PAR, vector<int32_t>(F, 0)));

    printf("Monitor %s\n", name.c_str());
    for(int ii = 0; ii < II; ii++) {
        for(int p = 0; p < PAR; p++) {
            array<T,F> data;
            inputStream[p] >> data;
            outputStream[p] << data;
            for(int f = F-1; f >= 0; f--) {
                printf("%02x",data[f].range(T::width-1,0).to_int());
                slice[0][ii*PAR+p][f] = data[f].range(T::width-1,0).to_int();
            }
            printf("\n");
        }
    }

    std::string path = std::string(name) + std::string(".h5");
    std::ifstream file(path);
    if (!file.good()) {
        HighFive::File file(path, HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);
        HighFive::DataSpace dataspace = HighFive::DataSpace({1, II*PAR, F}, {HighFive::DataSpace::UNLIMITED, II*PAR, F});
        HighFive::DataSetCreateProps props;
        props.add(HighFive::Chunking(std::vector<hsize_t>{1, II*PAR, F}));
        HighFive::DataSet dataset = file.createDataSet<int32_t>(name, dataspace,props);
        dataset.select({0, 0, 0}, {1, II*PAR, F}).write(slice);
    } else {
        HighFive::File file(path, HighFive::File::ReadWrite);
        auto dataset = file.getDataSet(name);
        auto size = dataset.getSpace().getDimensions();
        dataset.resize({size[0]+1 ,II*PAR, F});
        dataset.select({size[0], 0, 0}, {1, II*PAR, F}).write(slice);
    }
}

template<typename T,
         int F,
         int II>
void monitor(hls::stream<array<T,F>> &inputStream,
             hls::stream<array<T,F>> &outputStream,
             std::string name) {
    std::vector<std::vector<std::vector<double>>> slice(1, std::vector<std::vector<double>>(II, std::vector<double>(F, 0.0)));
    for(int ii = 0; ii < II; ii++) {
        array<T,F> data;
        inputStream >> data;
        outputStream << data;
        for(int f = 0; f < F; f++) {
            slice[0][ii][f] = static_cast<double>(data[f]);
        }
    }

    std::string path = std::string(name) + std::string(".h5");
    std::ifstream file(path);
    if (!file.good()) {
        HighFive::File file(path, HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);
        HighFive::DataSpace dataspace = HighFive::DataSpace({1, II, F}, {HighFive::DataSpace::UNLIMITED, II, F});
        HighFive::DataSetCreateProps props;
        props.add(HighFive::Chunking(std::vector<hsize_t>{1, II, F}));
        HighFive::DataSet dataset = file.createDataSet<double>("features", dataspace,props);
        dataset.select({0, 0, 0}, {1, II, F}).write(slice);
    } else {
        HighFive::File file(path, HighFive::File::ReadWrite);
        auto dataset = file.getDataSet("features");
        auto size = dataset.getSpace().getDimensions();
        dataset.resize({size[0]+1 ,II, F});
        dataset.select({size[0], 0, 0}, {1, II, F}).write(slice);
    }
}

#endif
