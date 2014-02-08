#include "in_json_2012.hpp"
#include "trigger_handler.hpp"
#include <cfloat>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

using namespace std;
const std::vector<std::vector<int> > VRunLumiPrompt(MakeVRunLumi("Golden"));
const std::vector<std::vector<int> > VRunLumi24Aug(MakeVRunLumi("24Aug"));
const std::vector<std::vector<int> > VRunLumi13Jul(MakeVRunLumi("13Jul"));

#define Ntriggers 1

void trigger_handler::CalTrigEfficiency(int Nentries, string outFilename){

  TFile outFile(outFilename.c_str(), "recreate");
  outFile.cd();

  TString TriggerName[][2] = {{"HLT_PFNoPUHT350_Mu15_PFMET45", "HLT_Mu40"}};
  int nBins[] = {30};
  float Range[][2] = {{0, 600}};

  TH1F *hTrigEff[Ntriggers][3];
  TString hName, hisPrefix[] = {"Num_", "Den_", "Eff_"};
  for(int tri(0); tri < Ntriggers; tri++){
    for(int his(0); his < 3; his++){
      hName = hisPrefix[his]; hName += TriggerName[tri][0]; hName += "_ref_";
      hName += TriggerName[tri][1];
      hTrigEff[tri][his] = new TH1F(hName, hName, nBins[tri], Range[tri][0], Range[tri][1]);
    }
  }

   for(int entry(0); entry < Nentries; entry++){
     GetEntry(entry);

     if(!PassesJSONCut()) continue;
     if(!PassesPVCut()) continue;
     if(!PassesMETCleaningCut()) continue;

   }

  outFile.Write();
  outFile.Close();
  cout<<"Finished saving file "<<outFilename<<endl;
}

bool trigger_handler::PassesMETCleaningCut() const{
//   for(unsigned int jet(0); jet<jets_AK5PF_pt->size(); ++jet){
//     if(isProblemJet(jet)) return false;
//   }
//   if(pfTypeImets_et->at(0)>2.0*mets_AK5_et->at(0)) return false; 
  return cschalofilter_decision
    && hbhefilter_decision
    && hcallaserfilter_decision 
    && ecalTPfilter_decision 
    && trackingfailurefilter_decision 
    && eebadscfilter_decision 
    && scrapingVeto_decision;
//     && greedymuonfilter_decision 
//     && PassesBadJetFilter()
//     && inconsistentPFmuonfilter_decision 
//     && (ecallaserfilter_decision || sampleName.find("_v66")!=std::string::npos)
//     && GetPBNR()>=1;
}

bool trigger_handler::PassesPVCut() const{
  if(beamSpot_x->size()<1 || pv_x->size()<1) return false;
  const double pv_rho(sqrt(pv_x->at(0)*pv_x->at(0) + pv_y->at(0)*pv_y->at(0)));
  if(pv_ndof->at(0)>4 && fabs(pv_z->at(0))<24. && pv_rho<2.0 && pv_isFake->at(0)==0) return true;
  return false;
}

bool trigger_handler::PassesJSONCut() const {
  if(sampleName.find("Run2012")!=std::string::npos){
    if(sampleName.find("PromptReco")!=std::string::npos
       &&!inJSON(VRunLumiPrompt, run, lumiblock)) return false;
    if(sampleName.find("24Aug")!=std::string::npos
       && !inJSON(VRunLumi24Aug, run, lumiblock)) return false;
    if(sampleName.find("13Jul")!=std::string::npos
       && !inJSON(VRunLumi13Jul, run, lumiblock)) return false;
    return true;
  }else{
    return true;
  }
}



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

