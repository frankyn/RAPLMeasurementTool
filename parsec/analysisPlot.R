# Read in Arguments
args <- commandArgs(trailingOnly = TRUE)
print(args)

# Read in out file 
data <- read.table(args[1], header = TRUE, sep=",", skip=1)

# Setup for Plot
epochs <- nrow(data)
x_range <- range(0, epochs)

max_energy_consumed <- max(0,data$PK.J)
min_energy_consumed <- min(data$PK.J)
energy_range <- range(min_energy_consumed, max_energy_consumed+5)

avg_freq <- (data$F0/1000+data$F1/1000+data$F2/1000+data$F3/1000+data$F4/1000+data$F5/1000+data$F6/1000+data$F7/1000)/8
max_freq <- max (avg_freq)
min_freq <- min (avg_freq)
freq_range <- range(min_freq, max_freq+.05 )

max_usage <- max( data$TU. )
usage_range <- range(0, max_usage+10)

setEPS()
postscript(paste("plots/",args[2]))

# Marginize plot
par(mar=c(5,12,4,4)+0.1)

#Energy plot
plot(data$PK.J, axes=F, ylim=energy_range, xlab="",ylab="",type="l",col="black",
	main="", xlim=x_range)

axis(2, ylim=energy_range, col="black", lwd=2)
mtext(2, text="Package Energy (J)", line=2)

# Freq plot
par(new=T)
plot(avg_freq, axes=F, ylim=freq_range, xlab="", ylab="",
        type="l", lty=2, main="", xlim=x_range, lwd=2)

axis(2, ylim=freq_range,  lwd=2, line=3.5)
mtext(2, text="Average CPU Frequency (GHz)", line=5.5)


# Usage plot
par(new=T)
plot(data$TU., axes=F, ylim=usage_range, xlab="", ylab="",
	type="l", lty=3, main="", xlim=x_range, lwd=2)
axis(2, ylim=usage_range, lwd=2,line=7)

mtext(2,text="CPU Usage (%)",line=9)


# Epoch Axis
axis(1, pretty(x_range, 10))
mtext("Epoch (1s)", side=1, col="black", line=2)

# Plot Legend

legend ( 1,y=max_usage+10, legend=c("Package Energy", "Average CPU Frequency", "CPU Usage"), lty=c(1,2,3), cex=0.50,horiz=TRUE)

dev.off()




