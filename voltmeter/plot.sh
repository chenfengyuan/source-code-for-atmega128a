#!/bin/sh
#set fit logfile  '/dev/shm/fit.log'
#f(x)=a*x+b;
#fit f(x) $datafile via a,b
#plot $datafile using 1:2# w lp,f(x)
# plot '/dev/shm/a' using 1:(f($1)-$2) w lp
echo $datafile > /dev/shm/aoeu
