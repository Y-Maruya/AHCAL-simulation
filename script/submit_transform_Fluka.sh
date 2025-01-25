#!/bin/bash

for i in {0..99}
do
    # ./convert /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output_${i}.h5 
    ./convert_withdigi_wg_Fluka /afs/cern.ch/user/y/ymaruya/private/FASERlink/Fluka_Read/Fluka.gfaser_split_${i}_calo.root /afs/cern.ch/user/y/ymaruya/private/FASERlink/Fluka_Read/Fluka.gfaser_split_${i}_withdigi_wg.h5 
    # ./analysis /afs/cern.ch/user/y/ymaruya/private/FASERlink/Fluka_Read/Fluka.gfaser_split_${i}_calo.root /afs/cern.ch/user/y/ymaruya/private/FASERlink/Fluka_Read/Fluka.gfaser_split_${i}_withdigi_wg.root
    # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
done