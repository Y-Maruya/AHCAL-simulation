#!/bin/bash

for i in {0..5}
do
    ./convert_withdigi_wg_hadron /afs/cern.ch/user/y/ymaruya/private/FASERlink/FASER_neBkg/NeutralBkgflux_${i}.gfaser_calo.root /afs/cern.ch/user/y/ymaruya/private/FASERlink/FASER_neBkg/NeutralBkgflux_${i}.h5
    ./analysis /afs/cern.ch/user/y/ymaruya/private/FASERlink/FASER_neBkg/NeutralBkgflux_${i}.gfaser_calo.root /afs/cern.ch/user/y/ymaruya/private/FASERlink/FASER_neBkg/NeutralBkgflux_${i}.ana.root
done