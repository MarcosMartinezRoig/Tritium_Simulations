#!/bin/bash

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup geant4 v4_10_6_p01 -q e19:prof
setup cmake v3_14_3

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

RunNumber=$((Ini+$1))

AcquisitionTime=600
SourceAcquivity=450 #Bq/L

randSeed=$RANDOM

FOLDER=DATA

OUTPUT_FILE="MultipleFibbers_"$SourceAcquivity"BqL_Run_"$RunNumber".root"

echo "OUTPUT_FILE: " $OUTPUT_FILE

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

./MultipleFibbers run1.mac $RunNumber 0 $AcquisitionTime $SourceAcquivity $randSeed

setup root v6_18_04d -q e19:prof

#root -q -l Analise.C\(\"MultipleFibbers_"$SourceAcquivity"BqL_Run_"$1".root\"\)

echo "ls -al: "
ls -al

echo "stat file.root: "
stat $OUTPUT_FILE

root -q -l Analise.C\(\"$OUTPUT_FILE\"\)

#mv *.root $WORKING_DIRECTORY/$FOLDER
