Universe = Vanilla
Getenv = True
Executable = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/bin/SimCalModule_Gx650
Arguments = /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/mac/run_off_AHCAL_Gx650_max_$(Process).mac
+JobFlavour = "testmatch"
Output = out/run_off_AHCAL_Gx650_max_$(Process).out
Error = err/run_off_AHCAL_Gx650_max_$(Process).err
Log = log/run_off_AHCAL_Gx650_max_$(Process).log
Queue 10
