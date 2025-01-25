#!/bin/bash

for i in {0..5}
do
    # ./convert /eos/user/y/ymaruya/FASER/genie_data/faser_off_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_charm.1000ifb.${i}.h5 
    # ./convert_withdigi_wg_hadron /eos/user/y/ymaruya/FASER/FASER_neBkg/NeutralBkgflux_${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/FASER_neBkg/NeutralBkgflux_${i}.withdigi_wg.h5
    ./analysis /eos/user/y/ymaruya/FASER/FASER_neBkg/NeutralBkgflux_${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/FASER_neBkg/NeutralBkgflux_${i}.withdigi_wg.root
    # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
done
