#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup geant4 v4_10_6_p01 -q e19:prof
setup cmake v3_14_3
setup root v6_18_04d -q e19:prof

#Added by Marcos -> CRY library of Geant4
#export G4INSTALL=/cvmfs/larsoft.opensciencegrid.org/products/geant4/v4_10_6_p01/Linux64bit+3.10-2.17-e19-prof
#export G4SYSTEM=Linux-g++
#export G4WORKDIR=$HOME/geant4_workdir
#source /lhome/ific/m/marcosmr/20cm1mmFibers_without_Lead_without_VETO/cry_v1.7/setup

WORKING_DIRECTORY=$(pwd)

echo "Working directory: " $WORKING_DIRECTORY

echo "Hostname: "
hostname

echo "id: "
id

Ini=6696

RunNumber1=$1
RunNumber2=$((Ini+$1))

AcquisitionTime=600
SourceAcquivity=600 #Bq/L

randSeed=$RANDOM

FOLDER=DATA

OUTPUT_FILE1="MultipleFibbers_"$SourceAcquivity"BqL_Run_"$RunNumber1".root"
OUTPUT_FILE2="MultipleFibbers_"$SourceAcquivity"BqL_Run_"$RunNumber2".root"

echo "OUTPUT_FILE1: " $OUTPUT_FILE1
echo "OUTPUT_FILE2: " $OUTPUT_FILE2

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

./MultipleFibbers run1.mac $RunNumber1 0 $AcquisitionTime $SourceAcquivity $randSeed
./MultipleFibbers run1.mac $RunNumber2 0 $AcquisitionTime $SourceAcquivity $randSeed

#root -q -l Analise.C\(\"MultipleFibbers_"$SourceAcquivity"BqL_Run_"$1".root\"\)

echo "ls -al: "
ls -al

echo "stat file.root: "
stat $OUTPUT_FILE1
stat $OUTPUT_FILE2

root -q -l Analise.C\(\"$OUTPUT_FILE1\"\)
root -q -l Analise.C\(\"$OUTPUT_FILE2\"\)

#mv *.root $WORKING_DIRECTORY/$FOLDER
