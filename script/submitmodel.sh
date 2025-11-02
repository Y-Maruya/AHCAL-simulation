#!bin/bash
# for j in max min QGSJET SIBYLL
for j in central max min EPOSLHC QGSJET SIBYLL
do
    # for i in {0..9}
    # do
    #     ./convert_withdigi_wg /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCAL0mmW_charm.${j}.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCAL0mmW_charm.${j}.1000ifb.${i}.ord.h5
    #     # ./convert /eos/user/y/ymaruya/FASER/genie_data/faser_off_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_charm.1000ifb.${i}.h5 
    #     # ./convert_withdigi /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCAL${j}mmW_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCAL${j}mmW_charm.1000ifb.withdigi.${i}.h5 
    #     # ./convert_withdigi_wg /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_charm.1000ifb.withdigi_wg.${i}.h5 
    # # ./analysis /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_charm.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_charm.1000ifb.withdigi_wg.${i}.root
    #     # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
    # done
    # for i in {10..19}
    # do
    #     ./convert_withdigi_wg /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCAL0mmW_light.${j}.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCAL0mmW_light.${j}.1000ifb.${i}.ord.h5
    # # ./convert /eos/user/y/ymaruya/FASER/genie_data/faser_off_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_light.1000ifb.${i}.h5 
    # # ./convert_withdigi /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_light.1000ifb.withdigi.${i}.h5 
    #     # ./convert_withdigi_wg /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_light.1000ifb.withdigi_wg.${i}.h5
    # # ./analysis /eos/user/y/ymaruya/FASER/genie_data/faser_off_AHCALwg_light.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/faser_off_AHCALwg_light.1000ifb.withdigi_wg.${i}.root
    # # ./convert_random /eos/user/y/ymaruya/FASER/genie_data/faser_on.1000ifb.${i}.gfaser_calo.root /eos/user/y/ymaruya/FASER/gfaser_calo/output.random_${i}.h5 
    # done
    condor_submit submit_off_model_${j}_muon.sdf
    
done