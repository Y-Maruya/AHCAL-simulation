#!/bin/bash

for i in {400..999}
do
    ./convert /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output_${i}.h5 
    # ./convert_withdigi /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.withdigi_${i}.h5 
    # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
done