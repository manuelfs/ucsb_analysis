// utils_macros: Various utilities used in the macros but not in the main code

#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"

#define NHis 4

using namespace std;
using std::cout;
using std::endl;

void ReadChains(TChain *chain[2][NHis]){
  TString FileNames[2][NHis] = {{"archive/TT_CT10_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1881_v71_.root",
				 "archive/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68_1150_525.root",
				 "archive/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-25to500_50GeVX50GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1732reshuf_v68_1400_25.root",
				 "archive/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68_1400_525.root"},
				{"archive/TTbar_TuneZ2star_13TeV-powheg-tauola_Summer13dr53X-PU25bx25_START53_V19D-v1_AODSIM_UCSB2027_v71_.root",
				 "archive/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1145_500.root",
				 "archive/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1500_1_.root",
				 "archive/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1500_500.root"}};
  for(int ene(0); ene < 2; ene++){
    for(int his(0); his < NHis; his++){
      chain[ene][his] = new TChain("tree");
      chain[ene][his]->Add(FileNames[ene][his]);
    }
  }
}
