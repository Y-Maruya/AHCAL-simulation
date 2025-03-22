#!/bin/bash

for i in {1..300}
do
    cd /afs/cern.ch/user/y/ymaruya/private/FASERlink/AHCAL-analyse/
    # ./convert /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output_${i}.h5 
    /afs/cern.ch/user/y/ymaruya/private/FASERlink/AHCAL-analyse/bin/convert /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.withdigi_r0000_${i}.h5 
    # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
    cd /afs/cern.ch/user/y/ymaruya/private/AHCAL-simulation/script/
done