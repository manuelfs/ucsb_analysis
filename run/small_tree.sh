#! /bin/csh

set Nevents=5000000
#export Nevents=5000000
#if (( "$#" >= 1 )); then
#    export Nevents=$1
#fi

# QCD @13TeV: 2,700,637 entries
bsub ./run/make_tree.exe -i /net/cms26/cms26r0/jaehyeok/QCD_Pt-300to470_Tune4C_13TeV_pythia8_Spring14miniaod-castor_PU20bx25_POSTLS170_V5-v1_MINIAODSIM -n $Nevents  | tee log/maketree_13TeV_QCT_Pt-800to1000.log 

exit 0;
