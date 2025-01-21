#!/bin/bash

for i in {7..9}
do
    # ./convert /eos/user/y/ymaruya/FASER/genie_data/faser_off_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_charm.1000ifb.${i}.h5 
    ./convert_withdigi /afs/cern.ch/user/y/ymaruya/private/FASERlink/genie_data/faser_nue.${i}.gfaser_calo.root /afs/cern.ch/user/y/ymaruya/private/FASERlink/genie_data/faser_nue.withdigi.${i}.h5 
    # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
done
