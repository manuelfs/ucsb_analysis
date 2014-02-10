#! /bin/bash

export Nevents=1
if (( "$#" >= 1 )); then
    export Nevents=$1
fi

# ./run/calc_trig_efficiency.exe -i SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71 -n $Nevents  -m 1145_800 | tee log/make_14TeV_T1tttt.log &
# ./run/calc_trig_efficiency.exe -i SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67 -n $Nevents  | tee log/make_2012D.log &
 ./run/calc_trig_efficiency.exe -c -i /net/cms2/cms2r0/cfA/SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67/cfA_SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67_f999_1_AMZ.root -n $Nevents  | tee log/make_2012D.log &

wait
exit 0;
