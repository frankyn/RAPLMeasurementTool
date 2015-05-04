# Read in Arguments
args <- commandArgs(trailingOnly = TRUE)
print(args)

# Read in out file 
data <- read.table(args[1], header = TRUE, sep=",", skip=1)

# Setup for Plot
epochs <- nrow(data)
x_range <- range(0, epochs+15)

max_usage <- max(data$C0, data$C1, data$C2, data$C3, data$C4, data$C5, data$C6,data$C7)
y_range <- range(0, max_usage)

# Begin Plotting
setEPS()
postscript(paste("plots/",args[2]))

par(mar=c(5, 4, 16, 2) + 0.1, mgp=c(3,1,0))
plot(data$C0,col="red", type="l",xlim=x_range, ylim=y_range, ann=FALSE)

box()

lines(data$C1, col="blue")
lines(data$C2, col="purple")
lines(data$C3, col="green")
lines(data$C4, col="black")
lines(data$C5, col="gray")
lines(data$C6, col="pink")
lines(data$C6, col="brown")
# Axis Labels
title(xlab="Epoch (1s)", ylab="Core Usage (%)")


# Plot Legend
legend(epochs+5,y=max_usage, c("C0","C1","C2","C3","C4","C5","C6","C7"), lty=c(1,1,1,1,1,1,1,1), col=c("red","blue","purple","green","black","gray","pink","brown"))

# Complete plotting
dev.off()
