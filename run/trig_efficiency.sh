#! /bin/bash

export Nevents=1
if (( "$#" >= 1 )); then
    export Nevents=$1
fi

# Signal @14TeV
 ./run/calc_trig_efficiency.exe -i SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71 -n $Nevents  -m 1145_500 | tee log/make_14TeV_T1tttt_500.log &

# Signal @14TeV: 40,321 entries
 ./run/calc_trig_efficiency.exe -i SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71 -n $Nevents  -m 1145_800 | tee log/make_14TeV_T1tttt.log &
# Signal @8TeV: 127,823 entries
 ./run/calc_trig_efficiency.exe -i SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68 -n $Nevents  -m 1145_800 | tee log/make_8TeV_T1tttt.log &

# All ttbar @13TeV: 997,120 entries
 ./run/calc_trig_efficiency.exe -i TTbar_TuneZ2star_13TeV-pythia6-tauola_Summer13dr53X-PU45bx25_START53_V19D-v2_AODSIM_UCSB1933_v71 -n $Nevents  | tee log/make_13TeV_ttbar.log &
# Leptonic ttbar @13TeV: 12,011,428 entries
 ./run/calc_trig_efficiency.exe -i TTJets_FullLeptMGDecays_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7C-v2_AODSIM_UCSB1883_v71 -n $Nevents  | tee log/make_8TeV_ttbar_lep.log &
# Hadronic ttbar @13TeV: 31,223,821 entries
 ./run/calc_trig_efficiency.exe -i TTJets_HadronicMGDecays_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A_ext-v1_AODSIM_UCSB1880_v71 -n $Nevents  | tee log/make_8TeV_ttbar_had.log &
# Semi-leptonic ttbar @13TeV: 24,953,451 entries
 ./run/calc_trig_efficiency.exe -i TTJets_SemiLeptMGDecays_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7C-v1_AODSIM_UCSB1884_v71 -n $Nevents  | tee log/make_8TeV_ttbar_semilep.log &

# ./run/calc_trig_efficiency.exe -i SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67 -n $Nevents  | tee log/make_2012D.log &
# ./run/calc_trig_efficiency.exe -c -i /net/cms2/cms2r0/cfA/SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67/cfA_SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67_f999_1_AMZ.root -n $Nevents  | tee log/make_2012D.log &
# ./run/calc_trig_efficiency.exe -i SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67 -m v67_f997 -n $Nevents  | tee log/make_2012D.log &

#wait
exit 0;
