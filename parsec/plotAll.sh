#!/bin/sh

# All Threads
Rscript packageEnergyPlot.R data/BLACKSCHOLES_N_1_I_Native.out data/BLACKSCHOLES_N_4_I_Native.out data/BLACKSCHOLES_N_8_I_Native.out data/BLACKSCHOLES_N_16_I_Native.out "Blackscholes -- Energy Consumption"

Rscript packageEnergyPlot.R data/X264_N_1_I_Native.out data/X264_N_4_I_Native.out data/X264_N_8_I_Native.out data/X264_N_16_I_Native.out "x264 -- Energy Consumption"

Rscript packageEnergyPlot.R data/CANNEAL_N_1_I_Native.out data/CANNEAL_N_4_I_Native.out data/CANNEAL_N_8_I_Native.out data/CANNEAL_N_16_I_Native.out "Canneal -- Energy Consumption"

# Energy Per Thread
Rscript energyPlot.R data/BLACKSCHOLES_N_8_I_Native.out "Blackscholes 8 Thread -- Energy Consumption"

Rscript energyPlot.R data/X264_N_8_I_Native.out "X264 8 Thread -- Energy Consumption"

Rscript energyPlot.R data/X264_N_32_I_Native.out "X264 32 Thread -- Energy Consumption"

Rscript energyPlot.R data/CANNEAL_N_8_I_Native.out "CANNEAL 8 Thread -- Energy Consumption"

# Plot Freq
Rscript freqPlot.R data/BLACKSCHOLES_N_8_I_Native.out Blacscholes_Freq_N_8_Plot

Rscript freqPlot.R data/X264_N_8_I_Native.out X264_Freq_N_8_Plot

Rscript freqPlot.R data/CANNEAL_N_8_I_Native.out CANNEAL_Freq_N_8_Plot

# Plot Usage

Rscript usagePlot.R data/BLACKSCHOLES_N_8_I_Native.out Blackscholes_Usage_N_8_Plot

Rscript usagePlot.R data/X264_N_8_I_Native.out X264_Usage_N_8_Plot

Rscript usagePlot.R data/CANNEAL_N_8_I_Native.out CANNEAL_Usage_N_8_Plot

# Analysis 
Rscript analysisPlot.R data/BLACKSCHOLES_N_8_I_Native.out BLACKSCHOLES_analysis_N_8_Plot

Rscript analysisPlot.R data/X264_N_8_I_Native.out X264_analysis_N_8_Plot

Rscript analysisPlot.R data/CANNEAL_N_8_I_Native.out CANNEAL_analysis_N_8_Plot


