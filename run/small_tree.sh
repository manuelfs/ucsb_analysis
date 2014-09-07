#! /bin/csh

set Nevents=5000000
#if (( "$#" >= 1 )); then
#    set Nevents=$1
#fi

# T1tttt @ 13 TeV
bsub ./run/make_tree.exe -i /cms2r0/cfA/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2085_v73 -n $Nevents  | tee log/maketree_13TeV_T1tttt_1200.log
bsub ./run/make_tree.exe -i /cms2r0/cfA/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2120_v73  -n $Nevents  | tee log/maketree_13TeV_T1tttt_1500.log

# tt @ 13 TeV
bsub ./run/make_tree.exe -i /cms2r0/johnbr/cfA/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v2-MINIAODSIM_cfA_v72 -n $Nevents  | tee log/maketree_13TeV_tt.log

# QCD @13TeV: 2,700,637 entries
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-5to10_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM     -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-5to10.log
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-30to50_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM    -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-30to50.log
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-50to80_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM    -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-50to80.log
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-80to120_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM   -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-80to120.log 
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-300to470_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM  -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-300to470.log
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-470to600_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM  -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-470to600.log
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-600to800_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM  -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-600to800.log
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-800to1000_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-800to1000.log
bsub ./run/make_tree.exe -i /cms26r0/jaehyeok/QCD_Pt-3200_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM      -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-3200.log

bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-10to15_Tune4C_13TeV_pythia8      -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-10to15.log
bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-15to30_Tune4C_13TeV_pythia8      -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-15to30.log
bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-120to170_Tune4C_13TeV_pythia8    -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-120to170.log
bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-170to300_Tune4C_13TeV_pythia8    -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-170to300.log
bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8  -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-1000to1400.log
bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-1400to1800_Tune4C_13TeV_pythia8  -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-1400to1800.log
bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-1800to2400_Tune4C_13TeV_pythia8  -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-1800to2400.log
bsub ./run/make_tree.exe -i /cms6r0/CRAB_output/johnbr/QCD_Pt-2400to3200_Tune4C_13TeV_pythia8  -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-2400to3200.log

# W + jets @ 13 TeV
bsub ./run/make_tree.exe -i /cms2r0/cfA/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2097_v73 -n $Nevents  | tee log/maketree_13TeV_WJetsToLNu_HT-100to200.log
bsub ./run/make_tree.exe -i /cms2r0/cfA/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2096_v73 -n $Nevents  | tee log/maketree_13TeV_WJetsToLNu_HT-200to400.log
bsub ./run/make_tree.exe -i /homes/manuelf/links/files/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola -n $Nevents  | tee log/maketree_13TeV_WJetsToLNu_HT-400to600.log
bsub ./run/make_tree.exe -i /homes/manuelf/links/files/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola -n $Nevents  | tee log/maketree_13TeV_WJetsToLNu_HT-600toinf.log   


exit 0;
