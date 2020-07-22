#!/bin/bash

NR_CPUS=$(grep -c ^processor /proc/cpuinfo)
echo 'Number of CPUs: ' $NR_CPUS

rm *Single*.root
#rm AllGraphs.root

Graphs(){
	echo  $1
	root -q -l MakeSingleGraphs.C\("\"$1\""\)
}


export -f Graphs
parallel --jobs $NR_CPUS Graphs {} ::: AllDataSmall*.root

#root -l JoinGraphs.C

echo "DONE"



exit
