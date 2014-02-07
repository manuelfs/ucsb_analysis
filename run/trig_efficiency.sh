#! /bin/bash

export Nevents=1000000
./run/calc_trig_efficiency.exe -i SMS-T1tttt_2J_mGo-825_mLSP-225_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V19-v1_AODSIM_UCSB1788reshuf_v68 -n $Nevents | tee log/make_8TeV-T1tttt.log &







wait

echo "Done"
exit 0;
