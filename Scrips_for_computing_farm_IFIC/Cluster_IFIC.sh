#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup geant4 v4_10_6_p01 -q e19:prof
setup cmake v3_14_3

WORKING_DIRECTORY=$(pwd)

AcquisitionTime=60 
SourceAcquivity=100 #Bq/L

randSeed=$RANDOM

FOLDER=DATA

#SBATCH --nodes=1\n\
#SBATCH --tasks-per-node=1\n\
#SBATCH --ntasks=1\n\
#SBATCH --cpus-per-task=1\n\
#SBATCH --time=120\n\
#SBATCH --mem=600\n\
#SBATCH --partition=short\n\
#SBATCH--job-name="+resultsFile+"\n\
#SBATCH -o "+WORKING_DIRECTORY+"/OUTPUTS/"+resultsFile+".out\n\
#SBATCH -e "+WORKING_DIRECTORY+"/ERRORS/"+resultsFile+".err\n\
#SBATCH --mail-user=marcos.martinez@ific.uv.es\n\
#SBATCH --mail-type=FAIL\n\

if [ -d $FOLDER ];
	then
		echo 1
	else
		mkdir $FOLDER
fi

./MultipleFibbers run1.mac $1 0 $AcquisitionTime $SourceAcquivity $randSeed

setup root v6_18_04d -q e19:prof

#root -q -l Analise.C\(\"MultipleFibbers_"$SourceAcquivity"BqL_Run_"$1".root\"\)

root -q -l Analise.C\(\"MultipleFibbers_"$SourceAcquivity"BqL_Run_"$1".root\"\)

mv *.root $WORKING_DIRECTORY/$FOLDER
