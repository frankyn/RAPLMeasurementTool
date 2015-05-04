# Read in Arguments
args <- commandArgs(trailingOnly = TRUE)
print(args)

# Read in out file 
data <- read.table(args[1], header = TRUE, sep=",", skip=1)

# Setup for Plot
epochs <- nrow(data)
x_range <- range(0, epochs+30)

max_freq_consumed <- max(data$F0/1000,data$F1/1000,data$F2/1000,data$F3/1000,data$F4/1000,data$F5/1000,data$F6/1000,data$F7/1000)

min_freq_consumed <- min(data$F0/1000,data$F1/1000,data$F2/1000,data$F3/1000,data$F4/1000,data$F5/1000,data$F6/1000,data$F7/1000)


y_range <- range(min_freq_consumed, max_freq_consumed+.01)

# Begin Plottingi
# Start EPS File
setEPS()
postscript(paste("plots/",args[2]))

#png(filename=paste("plots/",args[2]), height=495, width=600, bg="white")

par(mar=c(5, 4, 16, 2) + 0.1, mgp=c(3,1,0))
plot(data$F0/1000,col="red", type="l",xlim=x_range, ylim=y_range, ann=FALSE)

#box()

lines(data$F1/1000, col="blue")
lines(data$F2/1000, col="purple")
lines(data$F3/1000, col="green")
lines(data$F4/1000, col="black")
lines(data$F5/1000, col="gray")
lines(data$F6/1000, col="pink")
lines(data$F7/1000, col="brown")

# Axis Labels
title(xlab="Epoch (1s)", ylab="Frequency (GHz)")


# Plot Legend
legend(epochs+5,y=max_freq_consumed, c("C0","C1","C2","C3","C4","C5","C6","C7"), lty=c(1,1,1,1,1,1,1,1), col=c("red","blue","purple","green","black","gray","pink","brown"))

# Complete plotting
dev.off()
