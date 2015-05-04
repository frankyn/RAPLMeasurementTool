# Read in Arguments
args <- commandArgs(trailingOnly = TRUE)
print(args)

# Read in out file 
data <- read.table(args[1], header = TRUE, sep=",", skip=1)

# Setup for Plot
epochs <- nrow(data)
x_range <- range(0, epochs)

max_energy_consumed <- max(data$PK.J, data$DRAM.J, data$EC.J)
y_range <- range(0, max_energy_consumed+20)

# Begin Plotting
# png(filename=paste("plots/",args[2]), height=495, width=600, bg="white")
setEPS()
postscript(paste("plots/",args[2]))
#png(filename=paste("plots/",args[2]))

plot(data$PK.J,col="red", type="l",xlim=x_range, ylim=y_range, ann=FALSE)

box()

lines(data$DRAM.J, col="blue")
lines(data$EC.J,col="purple")

# Axis Labels
title(main=args[2],xlab="Epoch (1s)", ylab="Energy Consumed (J)")


# Plot Legend
legend(1,y=max_energy_consumed+20, c("Package Energy","DRAM","PP0"), lty=c(1,1,1), col=c("red","blue","purple") )

# Complete plotting
dev.off()
