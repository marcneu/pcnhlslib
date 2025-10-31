# SBCCI 2025 PCN HLS Example

## Overview

This folder contains the design used in our GNNETM paper. We leverage our library of processing elements suitable for deploying graphg-based point cloud networks on FPGAs.

## C Simulation

### Preqrequisites

This package requires the following dependencies:

- hdf5 >=1.14.0
- HighFive >=2.8.0
- AMD Vitis >=2024.2

We assume that hdf5 and HighFive are installed under `$HOME/bin`. If you choose a different location please adjust the Makefiles or configuration files accordingly.
To perform bit-accurate simulation of the CaloClusterNet, you must first compile the simulation for your system. Please make sure that the AMD Vitis environment is loaded and execute:
```sh
make csim
``` 

Generated binary can be found under `build`

### Run

Our C Simulation provides a suitable interface for data-level verification and testing.
```sh
csim.exe -i <input_path>.h5 -o <output_path>.h5 -n <number_of_events>
```
As an input the simulation requires an hdf5 database with suitable events. 

## Build Flow

The current version of this repository is tested with AMD Vitis 2024.2

### Hardware Synthesis and Implementation Flow

Call `make synth` to run the HLS synthesis.