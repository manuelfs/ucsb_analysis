// utils_macros: Various utilities used in the macros but not in the main code

#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"

#define NSam 5
using namespace std;
using std::cout;
using std::endl;

void ReadChains(TChain *chain[]){
  TString FileNames[] = 
    {"archive/14-08-24/*T1ttt*1200_*", 
     "archive/14-08-24/*T1ttt*1200_*", 
     "archive/14-08-24/*TT*",
     "archive/14-08-24/*QCD*",
     "archive/14-08-24/*WJets*"};

  for(int sam(0); sam < NSam; sam++){
      chain[sam] = new TChain("tree");
      chain[sam]->Add(FileNames[sam]);
  }
}
