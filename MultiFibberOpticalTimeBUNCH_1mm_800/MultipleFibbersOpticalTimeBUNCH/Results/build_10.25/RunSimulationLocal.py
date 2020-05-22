#!/usr/bin/env python
import os
import time
import random
import multiprocessing  # the module we will be using for multiprocessing

AcquisitionTime=60;
SourceAcquivity=1000;#Bq/L
Run=0

def work(Run):
	#Run=Run+0 #para incrementar os que ja estao
	WORKING_DIRECTORY=os.getcwd()
	randSeed=random.randint(0,32766)
	resultsFile="MultipleFibbers_"+str(Run)
	os.system("mkdir "+resultsFile)
	os.system("cp "+WORKING_DIRECTORY+"/MultipleFibbers "+resultsFile)
	os.system("cp "+WORKING_DIRECTORY+"/*.mac "+resultsFile)
	os.system("cp "+WORKING_DIRECTORY+"/TRITIUMSpectrumNew.txt "+resultsFile)
	os.system("cp "+WORKING_DIRECTORY+"/Analise.C "+resultsFile)
	os.chdir(WORKING_DIRECTORY+"/"+resultsFile)
	os.system("./MultipleFibbers run1.mac "+str(Run)+" 0 "+str(AcquisitionTime)+" "+str(SourceAcquivity)+" "+str(randSeed)+" > output"+str(Run)+".txt")
	#os.system("./MultipleFibbers run1.mac "+str(Run)+" "+str(NEvents)+" "+str(randSeed) +" > output"+str(Run)+".txt")
	os.system("root -q -l Analise.C\(\\\"MultipleFibbers_"+str(SourceAcquivity)+"BqL_Run_"+str(Run)+".root\\\"\)")
	os.system("mv *.root "+WORKING_DIRECTORY+"/DATA/")
	#os.system("mv output*.txt "+WORKING_DIRECTORY+"/OUTPUTS/")
	os.chdir(WORKING_DIRECTORY)
	os.system("rm -rf "+resultsFile)
	print "Unit of work number %d" % Run  # simply print the worker's number

if __name__ == "__main__":  # Allows for the safe importing of the main module
	print("There are %d CPUs on this machine" % multiprocessing.cpu_count())
	os.system("mkdir DATA")
	os.system("mkdir OUTPUTS")
	number_processes = 10
	pool = multiprocessing.Pool(number_processes)
	total_tasks = 10
	tasks = range(total_tasks)
	results = pool.map_async(work, tasks)
	pool.close()
	pool.join()
	
