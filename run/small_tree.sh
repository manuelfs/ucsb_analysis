#! /bin/bash

export Nevents=5000000
if (( "$#" >= 1 )); then
    export Nevents=$1
fi

# Signal @8TeV: 82,571 entries
 ./run/make_tree.exe -i SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68 -n $Nevents  -m 1150_525 | tee log/maketree_8TeV_T1tttt.log 
# Signal @14TeV: 40,457 entries
 ./run/make_tree.exe -i SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71 -n $Nevents  -m 1145_500 | tee log/maketree_14TeV_T1tttt.log 

# Signal @8TeV: 137,498 entries
 ./run/make_tree.exe -i SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-25to500_50GeVX50GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1732reshuf_v68 -n $Nevents  -m 1400_25 | tee log/maketree_8TeV_T1tttt_1400_25.log 
# Signal @14TeV: 41,575 entries
 ./run/make_tree.exe -i SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71 -n $Nevents  -m 1500_1_ | tee log/maketree_14TeV_T1tttt_1500_1.log 

# Signal @8TeV: 127,172 entries
 ./run/make_tree.exe -i SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68 -n $Nevents  -m 1400_525 | tee log/maketree_8TeV_T1tttt_1400_525.log 
# Signal @14TeV: 42,376 entries
 ./run/make_tree.exe -i SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71 -n $Nevents  -m 1500_500 | tee log/maketree_14TeV_T1tttt_1500_500.log 

# Powheg ttbar @13TeV: 993,322 entries
 ./run/make_tree.exe -i TTbar_TuneZ2star_13TeV-powheg-tauola_Summer13dr53X-PU25bx25_START53_V19D-v1_AODSIM_UCSB2027_v71 -n $Nevents  | tee log/maketree_13TeV_ttbar_25PU.log 
# Powheg ttbar @8TeV: 21,675,970 entries
 ./run/make_tree.exe -i TT_CT10_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1881_v71 -n $Nevents | tee log/maketree_8TeV_ttbar.log
# Powheg ttbar @8TeV: 1,144,808 entries
# ./run/make_tree.exe -i TT_CT10_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1881_v71 -n $Nevents -m _f9 | tee log/maketree_8TeV_ttbar.log

exit 0;
