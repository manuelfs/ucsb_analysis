// utils_macros: Various utilities used in the macros but not in the main code

#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"

#define NSam 5
using namespace std;
using std::cout;
using std::endl;

void ReadChains(TChain *chain[], int entries[]){
  TString folder = "archive/14-08-24/";
  TString FileNames[] = 
    {folder+"*T1ttt*1200_*", 
     folder+"*T1ttt*1500_*", 
     folder+"*TT*",
     folder+"*QCD*",
     folder+"*WJets*"};

  TChain *tglobal[NSam];
  for(int sam(0); sam < NSam; sam++){
      chain[sam] = new TChain("tree");
      tglobal[sam] = new TChain("treeglobal");
      chain[sam]->Add(FileNames[sam]);
      tglobal[sam]->Add(FileNames[sam]);
      tglobal[sam]->SetBranchAddress("noriginal",&entries[sam]);
      tglobal[sam]->GetEntry(0);
      tglobal[sam]->Delete();
  }
}
