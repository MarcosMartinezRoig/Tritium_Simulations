#!/usr/bin/env python

import os
import time
import random
import sys

#source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
#setup geant4 v4_10_6_p01 -q e19:prof
#setup cmake v3_14_3

WORKING_DIRECTORY=os.getcwd()

AcquisitionTime=60;
SourceAcquivity=100;#Bq/L

randSeed=random.randint(0,32766)
#resultsFile="MultipleFibbersBUNCH_"+str(sys.argv[1])

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

os.system("./MultipleFibbers run1.mac "+str(sys.argv[1])+" 0 "+str(AcquisitionTime)+" "+str(SourceAcquivity)+" "+str(randSeed)+"")

os.system("pwd")

#os.system("root -q -l Analise.C\(\\\"MultipleFibbers_100BqL_Run_0.root\\\"\)")

os.system("root -q -l Analise.C\(\\\"MultipleFibbers_"+str(SourceAcquivity)+"BqL_Run_"+str(sys.argv[1])+".root\\\"\)")

os.system("mv *.root "+WORKING_DIRECTORY+"/DATA/") #The DATA folder has to be created.
