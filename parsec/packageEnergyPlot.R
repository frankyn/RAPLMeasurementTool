# Read in Arguments
args <- commandArgs(trailingOnly = TRUE)
print(args)

# Read in out file 
thread1 <- read.table(args[1], header = TRUE, sep=",", skip=1)
thread4 <- read.table(args[2], header = TRUE, sep=",", skip=1)
thread8 <- read.table(args[3], header = TRUE, sep=",", skip=1)
thread16 <- read.table(args[4], header = TRUE, sep=",", skip=1)

# Setup for Plot
epochs <- max(c(nrow(thread1),nrow(thread4),nrow(thread8),nrow(thread16) ))
x_range <- range(0, epochs)

max_energy_consumed <- max(thread1$PK.J, thread4$PK.J, thread8$PK.J, thread16$PK.J)
y_range <- range(0, max_energy_consumed)
# Begin Plotting
setEPS()
postscript(paste("plots/",args[5]))

# png(filename=paste("plots/",args[5]), height=495, width=600, bg="white")


plot(thread1$PK.J,col="red", type="l",xlim=x_range,ylim=y_range ,ann=FALSE)

box()

lines(thread4$PK.J,  col="blue")
lines(thread8$PK.J,  col="purple")
lines(thread16$PK.J, col="black")

# Axis Labels
title(main=args[5],xlab="Epoch (1s)", ylab="Energy Consumed (J)")


# Plot Legend
legend(0,y=10, c("1 Thread","4 Threads", "8 Threads", "16 Threads"), lty=c(1,1,1), col=c("red","blue","purple","black") )

# Complete plotting
dev.off()
