#!/bin/bash

# for i in {0..9}
# do
#     ./convert_SciW /eos/user/y/ymaruya/FASER/genie_data/faser_onSciW_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_onSciW_charm.1000ifb.${i}.h5 
#     # ./convert_withdigi /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_charm.1000ifb.withdigi.${i}.h5 
#     # ./convert_withdigi_wg /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_charm.1000ifb.withdigi_wg.${i}.h5 
#     # ./analysis /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_charm.1000ifb.withdigi_wg.${i}.root
#     # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
# done
# for i in {10..19}
# do
#     ./convert_SciW /eos/user/y/ymaruya/FASER/genie_data/faser_onSciW_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_onSciW_light.1000ifb.${i}.h5 
# #     ./convert_withdigi /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_light.1000ifb.withdigi.${i}.h5 
#     # ./convert_withdigi_wg /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_light.1000ifb.withdigi_wg.${i}.h5
#     # ./analysis /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_light.1000ifb.withdigi_wg.${i}.root
# #     # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
# done
for i in {0..300}
do
    rm /eos/user/y/ymaruya/FASER/gfaser_calo/output_SciW_${i}.h5
    # ./convert_SciW /eos/user/y/ymaruya/FASER/genie_data/faser_onSciW_n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output_SciW_${i}.h5 
    # ./convert_withdigi /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.withdigi_${i}.h5 
    # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.n10000.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
done