# Real-Time Graph-based Point Cloud Networks on FPGAs via Stall-Free Deep Pipelining

This library contains architecture templates for deploying point cloud networks on FPGAs via AMD Vitis HLS. 
We provide High-Level Synthesis (HLS) architecture templates for common graph-based Point Cloud Network operators. Specifically we contribute templates for:

 - GraVNet operators
 - Condensation Point Selection operators.

Templates in this version are tested against AMD Vitis 2024.2. We have validated our examples on the VCK190, as well as AMD Ultrascale XCVU190.

## Examples

We provide two examples for our repository: GNN-ETM, and our SBCCI use case. You can find additional information in the respectives READMEs.
In general, you can expect all examples to work out of the box. However, if you would like to repdroduce the exact results, we recommend you to use the tagged commits (see below).

### GNN-ETM 

Please use the tagged commit [gnnetm](https://github.com/marcneu/pcnhlslib/tree/gnnetm), to reproduce our results exactly.

### SBCCI 2025 

Please use the tagged commit [sbcci25](https://github.com/marcneu/pcnhlslib/tree/sbcci25) to reproduce the exact results.

## Tests

Tests for developed kernels are available under `tests`.
You may build all unit test through `make all`.

## Citation

If you find this repository useful, please cite our work:

```
@inproceedings{11218652,
  author={Neu, Marc and Haide, Isabel and Justinger, Timo and Rädler, Till and Dajaku, Valdrin and Ferber, Torben and Becker, Jürgen},
  booktitle={2025 38th SBC/SBMicro/IEEE Symposium on Integrated Circuits and Systems Design (SBCCI)}, 
  title={Real-Time Graph-based Point Cloud Networks on FPGAs via Stall-Free Deep Pipelining}, 
  year={2025},
  volume={},
  number={},
  pages={1-5},
  keywords={Point cloud compression;Geometry;High energy physics;Graphics processing units;Detectors;Throughput;Real-time systems;Field programmable gate arrays;System analysis and design;Pipeline processing;Point Cloud Network;Graphs;FPGA;Real-Time;High-Performance-Computing;Dataflow;Latency},
  doi={10.1109/SBCCI66862.2025.11218652}}
```