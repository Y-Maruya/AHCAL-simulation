#!bin/bash
for j in central max min EPOSLHC QGSJET SIBYLL
do
    for i in Gx600 Gx650 Gx700
    do
        condor_submit submit_off_AHCAL_${i}_run2026_${j}.sdf
    done
done