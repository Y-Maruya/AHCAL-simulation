Universe = Vanilla
Getenv = True
Avalie = $(Process)+10
Jobid = $INT(Avalie)
Executable = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/bin/SimCalModule_Gx650
Arguments = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/mac/run_off_AHCAL_Gx650_SIBYLL_$(Jobid).mac
+JobFlavour = "testmatch"
Output = out/run_off_AHCAL_Gx650_SIBYLL_$(Jobid).out
Error = err/run_off_AHCAL_Gx650_SIBYLL_$(Jobid).err
Log = log/run_off_AHCAL_Gx650_SIBYLL_$(Jobid).log
Queue 10
