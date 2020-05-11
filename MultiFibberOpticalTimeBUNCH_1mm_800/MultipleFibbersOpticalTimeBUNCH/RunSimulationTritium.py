#!/usr/bin/env python
import os
import time
import random
WORKING_DIRECTORY=os.getcwd()
FirstRun=0
LastRun=0

AcquisitionTime=60;
SourceAcquivity=100;#Bq/L


for Run in range(FirstRun,LastRun):
	randSeed=random.randint(0,32766)
	resultsFile="MultipleFibbersBUNCH_"+str(Run)
	#Criar um ficheiro SUB para cada processo
	fileSUBName=resultsFile+".sub"
	ficheiroSUB=open(fileSUBName,"w")
	ficheiroSUB.write("#!/bin/bash\n\
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
#SBATCH --mail-user=cdazevedo@ua.pt\n\
#SBATCH --mail-type=FAIL\n\
mkdir "+resultsFile+"\n\
cp "+WORKING_DIRECTORY+"/MultipleFibbers "+resultsFile+"\n\
cp "+WORKING_DIRECTORY+"/*.mac "+resultsFile+"\n\
cp "+WORKING_DIRECTORY+"/TRITIUMSpectrum.txt "+resultsFile+"\n\
cp "+WORKING_DIRECTORY+"/Analise.C "+resultsFile+"\n\
cd "+resultsFile+"\n\
./MultipleFibbers run1.mac "+str(Run)+" 0 "+str(AcquisitionTime)+" "+str(SourceAcquivity)+" "+str(randSeed)+"\n\
root -q -l Analise.C\(\\\"MultipleFibbers_"+str(SourceAcquivity)+"BqL_Run_"+str(Run)+".root\\\"\)\n\
mv *.root "+WORKING_DIRECTORY+"/DATA/\n\
cd ..\n\
rm -rf "+resultsFile+"\n\
rm -rf "+resultsFile+".sub\n")
	ficheiroSUB.close()
	os.system("qsub "+fileSUBName)
