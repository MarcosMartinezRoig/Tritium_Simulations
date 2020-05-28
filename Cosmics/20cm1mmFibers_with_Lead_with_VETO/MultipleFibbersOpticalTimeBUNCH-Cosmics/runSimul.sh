#!/bin/bash

runSimul(){
  	echo ''
	echo 'Working in '$1
#	echo ''	
	var1=$(echo "nohup ./MultipleFibbers run1.mac $1 10 > run$1_nohup.txt &") # -q significa quit when done
	eval $var1
#	echo 'root file finished.'

#	echo 

}



WORKING_DIRECTORY=`pwd`

count=0
NR_CPUS=$(grep -c ^processor /proc/cpuinfo)
echo 'Number of CPUs: ' $NR_CPUS

NumberofJobs=1000
echo 'Number of Jobs for this work: ' $NumberofJobs


export -f runSimul
parallel --jobs $NumberofJobs runSimul {} ::: {34..44}
