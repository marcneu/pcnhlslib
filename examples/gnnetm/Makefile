export LD_LIBRARY_PATH += :${VITIS_HLS_ROOT}/lib/lnx64.o:${VITIS_HLS_ROOT}/lib/lnx64.o/Rhel/8

HDF5_LIB       := $$HOME/bin/hdf5-1_14_0
HIGHFIVE_LIB   := $$HOME/bin/highfive-2_8_0
VITIS_HLS_ROOT := /tools/xilinx/Vitis/2024.2

BUILD_ROOT 	         ?= build
LAYER_TEMPLATE_ROOT  := $(abspath ../../layers)
MODEL_TEMPLATE_ROOT  := $(abspath ../../models)
SRC_ROOT		     := $(abspath ./src)

CXX_SYNTH := v++ 

IFLAG_SIM += -g
IFLAG_SIM += -I "${LAYER_TEMPLATE_ROOT}" -I "${MODEL_TEMPLATE_ROOT}" 
IFLAG_SIM += -I "${VITIS_HLS_ROOT}/include"
IFLAG_SIM += -D__SIM_FPO__ -D__SIM_OPENCV__ -D__SIM_FFT__ -D__SIM_FIR__ -D__SIM_DDS__ -D__DSP48E1__
IFLAG_SIM += -L"${VITIS_HLS_ROOT}/lnx64/lib/csim" -Wl,-rpath,"${VITIS_HLS_ROOT}/lnx64/lib/csim" -lhlsmc++-CLANG39
IFLAG_SIM += -L"${VITIS_HLS_ROOT}/lnx64/tools/fpo_v7_1" -Wl,-rpath,"${VITIS_HLS_ROOT}/lnx64/tools/fpo_v7_1" -lIp_floating_point_v7_1_bitacc_cmodel -lgmp -lmpfr
IFLAG_SIM += -fuse-ld=lld -lm -lpthread
IFLAG_SIM += -L"$(HDF5_LIB)/lib" -lhdf5 -Wl,-rpath,"$(HDF5_LIB)/lib"

CFLAG_SIM += -g
CFLAG_SIM += -I "${LAYER_TEMPLATE_ROOT}" -I "${MODEL_TEMPLATE_ROOT}" 
CFLAG_SIM += -I "${VITIS_HLS_ROOT}/include"
CFLAG_SIM += -I "$(HDF5_LIB)/include"
CFLAG_SIM += -I "$(HIGHFIVE_LIB)/include"
CFLAG_SIM += -D__SIM_FPO__ -D__SIM_OPENCV__ -D__SIM_FFT__ -D__SIM_FIR__ -D__SIM_DDS__ -D__DSP48E1__

CFLAG_SIM += -fPIC -fPIE -Wno-unused-result
CFLAG_SIM += --gcc-toolchain=/tools/xilinx/Vitis/2024.2/tps/lnx64/gcc-8.3.0

CXX_SIM = ${VITIS_HLS_ROOT}/vcxx/libexec/clang++

HLS_CFG     = ../config/hls.cfg
TARGET = --part xcvu190-flgb2104-2-e

.PHONY: clean csim csynth link package all

clean:
	rm -f ${BUILD_ROOT}/*

${BUILD_ROOT}/csim.exe: ${SRC_ROOT}/csim.cpp ${SRC_ROOT}/caloclusternet.cpp ${MODEL_TEMPLATE_ROOT}/caloclusternetv3.h ${SRC_ROOT}/global.h ${SRC_ROOT}/weights.h
	mkdir -p ${BUILD_ROOT}/csim
	$(CXX_SIM) -c ${SRC_ROOT}/caloclusternet.cpp -o ${BUILD_ROOT}/csim/caloclusternet.o $(CFLAG_SIM)
	$(CXX_SIM) -c ${SRC_ROOT}/csim.cpp -o ${BUILD_ROOT}/csim/csim.o $(CFLAG_SIM)
	$(CXX_SIM) ${BUILD_ROOT}/csim/csim.o ${BUILD_ROOT}/csim/caloclusternet.o -o ${BUILD_ROOT}/csim.exe $(IFLAG_SIM)

csynth: ${SRC_ROOT}/caloclusternet.cpp ${MODEL_TEMPLATE_ROOT}/caloclusternetv3.h ${SRC_ROOT}/global.h ${SRC_ROOT}/weights.h
	mkdir -p $(BUILD_ROOT)
	cd $(BUILD_ROOT) && $(CXX_SYNTH) -c --mode hls $(TARGET) --config $(HLS_CFG)

csim: $(BUILD_ROOT)/csim.exe

all: package
