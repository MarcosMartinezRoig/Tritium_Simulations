#!/bin/bash

NR_CPUS=$(grep -c ^processor /proc/cpuinfo)
echo 'Number of CPUs: ' $NR_CPUS

rm AllDataSmall_5detectors*.root

makeData(){
	echo  $1
	root -q -l generate5detectors.C\("\"$1\""\)
}


export -f makeData
parallel --jobs $NR_CPUS makeData {} ::: AllDataSmall*.root


echo "DONE"



exit
