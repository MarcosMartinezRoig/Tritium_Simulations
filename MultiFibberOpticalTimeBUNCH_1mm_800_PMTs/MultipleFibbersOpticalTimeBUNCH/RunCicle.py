#!/usr/bin/env python
import os
import time
import re
import subprocess

WORKING_DIRECTORY=os.getcwd()
os.system("mkdir ERRORS")
os.system("mkdir OUTPUTS")
os.system("mkdir DATA")

while 1:
	proc = subprocess.Popen(["sacct -u cdazevedo | grep PENDING | wc -l", ""], stdout=subprocess.PIPE, shell=True)
	(QeuedRuns, err) = proc.communicate()
	print "Qeued Runs: "+QeuedRuns
	if int(QeuedRuns)<10:
		fileRun=open('RunSimulationTritium.py',"r")
		fileRunNew=open('RunSimulationTritium.py-NEW.py',"w")
		lines=fileRun.readlines()
		FirstRunLine=lines[5]
		LastRunLine=lines[6]
		OldFirstRun=int(re.findall('\\d+',FirstRunLine)[0])
		OldLastRun=int(re.findall('\\d+',LastRunLine)[0])
		if (OldLastRun>99):
			break;
		NewFirstRun=str(OldLastRun)
		NewLastRun=str(OldLastRun+50)
		NewFirstRunLine="FirstRun="+NewFirstRun+"\n"
		NewLastRunLine="LastRun="+NewLastRun+"\n"
		lines[5]=NewFirstRunLine
		lines[6]=NewLastRunLine
		print FirstRunLine
		print LastRunLine
		print lines[5]
		print lines[6]
		fileRunNew.writelines(lines)
		fileRun.close()
		fileRunNew.close()
		os.system("mv RunSimulationTritium.py-NEW.py RunSimulationTritium.py")
		os.system("python RunSimulationTritium.py")
	time.sleep(30)

