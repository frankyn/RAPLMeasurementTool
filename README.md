# RAPLMeasurementTool
A RAPL Measurement tool used to analyze power consumption to cpu usage per core.
> This is an approximation of percore energy consumption during each epoch (1 second).
> RAPL API developed by Intel doesn't maintain percore energy analysis.

# Requirements
RAPL capable processor from Intel, e.g. anything above SandyBridge should function.
We used Ubuntu 14.04.2 during this projects implementation. You must also install 'linux-tools-(kernel version)'. Every linux kernel has a precompiled set of binaries that handle RAPL API implementation without rebuilding your kernel. If you have linux-tools installed then run 'sudo modprobe msr'. You should see a new directory show up in '/dev/cpu/'; every subdirectory contains the Model Specific Registers used by the tool to determine energy consumption. 
 
# Compile / Build

> make 

# Clean up

> make clean

# Parsec Setup
  
  * [Execute] source /parsec\_path/env.sh
  * [Build] blackscholes, canneal, and x264
  * [Example] parsecmgmt -a build -p blackscholes

# Parsec Tests
 
  [Execute] ./batch.sh
  >[Output ] Located in parsec\_logs and plots..

# Parsec Plot 

  [Execute] ./plotAll.sh
  >[Output ] Located in plots

# About the collected Data

# Issues

Not all intel processors have RAPL features. If your processor is enabled then you must install required tools for RAPL and then use 'sudo modprobe msr'.
