#include "trigger_handler.hpp"
#include <cassert>
#include <climits>
#include <cfloat>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

using namespace std;

trigger_handler::trigger_handler(const std::string &fileName, const bool isList, const bool fastMode):
  cfA(fileName, isList){
  if (fastMode) { // turn off unnecessary branches
    chainA.SetBranchStatus("els_*",0);
    chainA.SetBranchStatus("triggerobject_*",0);
    chainA.SetBranchStatus("standalone_t*",0);
    chainA.SetBranchStatus("L1trigger_*",0);
    chainA.SetBranchStatus("passprescale*",0);
    chainA.SetBranchStatus("jets_AK5PFclean_*",0);
    chainA.SetBranchStatus("softjetUp_*",0);
    chainA.SetBranchStatus("pdfweights_*",0);
    chainA.SetBranchStatus("photon_*",0);
    chainB.SetBranchStatus("Ntcmets",0);
    chainB.SetBranchStatus("tcmets_*",0);
    chainB.SetBranchStatus("Nphotons",0);
    chainB.SetBranchStatus("photons_*",0);
    chainB.SetBranchStatus("Npf_photons",0);
    chainB.SetBranchStatus("pf_photons_*",0);
    chainB.SetBranchStatus("Nmus",0);
    chainB.SetBranchStatus("mus_*",0);
    chainB.SetBranchStatus("Nels",0);
    chainB.SetBranchStatus("els_*",0);
    chainB.SetBranchStatus("Nmets*",0);
    chainB.SetBranchStatus("mets*",0);
    chainB.SetBranchStatus("mets_AK5_et",1);
    chainB.SetBranchStatus("Njets_AK5PFclean",0);
    chainB.SetBranchStatus("jets_AK5PFclean_*",0);
    chainB.SetBranchStatus("Nmc*",0);
    chainB.SetBranchStatus("mc_*",0);
    chainB.SetBranchStatus("Nmc_doc*",1);
    chainB.SetBranchStatus("mc_doc*",1);
  }
}

void trigger_handler::CalTrigEfficiency(int Nentries){

  cout<<"Calculating trigger efficiency with "<<Nentries<<" entries."<<endl;
//   for(int entry(0); entry < Nentries; entry++){
//   }
}
