// var_optimization.C: Plots a FOM with respect to signal efficiency for cuts 
//                     on different variables

#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "inc/ucsb_utils.hpp"
#include "src/ucsb_utils.cpp"

#include "macros/utils_macros.cpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <ctime>
#include "TChain.h"
#include "TFile.h"
#include "TLine.h"
#include "TArrow.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TH1F.h"
#include "TMath.h"

#define NVar 4

using namespace std;
using std::cout;
using std::endl;

void divide_var(float cuts[], float fractions[], int nbins, vector<double_double> variable, double totw){
  double fbin = 1./(double)nbins, sumw(0), fw, prev_fw(0);
  double desired_fw = fbin;
  int bin = 1;
  for(unsigned int entry(0); entry<variable.size(); entry++){
    sumw += variable[entry].second;
    fw = sumw/totw;
    if(fw > desired_fw) {
      fractions[bin-1] = fw;
      for(int ibin(0); ibin<bin-1; ibin++) fractions[bin-1] -= fractions[ibin];
      if(fw-desired_fw < desired_fw-prev_fw) cuts[bin-1] = variable[entry].first;
      else {
	cuts[bin-1] = variable[entry-1].first;
	fractions[bin-1] -= variable[entry].second/totw;
      }
      bin++;
      if(bin==nbins) break;
      desired_fw = fbin*(double)bin;
    }
    prev_fw = fw;
  }
  cuts[nbins-1] = variable[variable.size()-1].first*10;
  fractions[nbins-1] = 1;
  for(int ibin(0); ibin<nbins-1; ibin++) fractions[nbins-1] -= fractions[ibin];  
}

void var_optimization(long fixnentries=-1){
  TChain *allchains[2][NHis], *chain;
  ReadChains(allchains);
  long nentries;

  //bool keep_greater[] = {true, true, false, true}; // Indicates whether we should keep <cut or >cut
  TString sVariable[] = {"spher", "mt", "dr_bb", "dphi_wlep"};
  vector<double_double> vVariable[NVar];
  float fVariable[NVar];
  double wVariable[NVar];
  float weight;

  time_t startTime, curTime;
  time(&startTime);
  for(int ene(0); ene < 1; ene++){
    // Setting up chain addresses
    chain = allchains[ene][1];
    if(chain->GetEntries() < fixnentries || fixnentries < 0) nentries = chain->GetEntries();
    else nentries = fixnentries;
    cout<<endl<<ene<<": Doing "<<nentries<<" entries"<<endl;
    chain->SetBranchAddress("weight", &weight);
    for(int var(0); var < NVar; var++){
      chain->SetBranchAddress(sVariable[var], &fVariable[var]);
      wVariable[var] = 0;
      vVariable[var].resize(0);
    }
    // Filling vector pairs
    for(int entry(0); entry<nentries; entry++){
      chain->GetEntry(entry);
      for(int var(0); var < NVar; var++){
	if(fVariable[var]>=0) {
	  vVariable[var].push_back(make_pair(fVariable[var], weight));
	  wVariable[var] += weight;
	}
      }
    } // Loop over all entries

    // Sorting and finding ranges
    for(int var(0); var < NVar; var++){
//       cout<<endl<<sVariable[var]<<" Before sorting: "<<vVariable[var][0].first<<", "<<vVariable[var][1].first<<", "
// 	  <<vVariable[var][vVariable[var].size()-1].first<<endl;
      sort(vVariable[var].begin(), vVariable[var].end(), dd_small2big);
      float cuts[10], fractions[10];
      int n = 10;
      divide_var(cuts, fractions, n, vVariable[var], wVariable[var]);
      cout<<"Bins: "<<RoundNumber(vVariable[var][0].first,2)<<", ";
      for(int ind(0); ind<n-1; ind++) cout<<RoundNumber(cuts[ind],2)<<", ";
      cout<<RoundNumber(vVariable[var][vVariable[var].size()-1].first,2)<<endl<<"\t ";
      for(int ind(0); ind<n; ind++) cout<<RoundNumber(fractions[ind]*100,1)<<", ";
      cout<<endl;
      n = 5;
      divide_var(cuts, fractions, n, vVariable[var], wVariable[var]);
      cout<<"Bins: "<<RoundNumber(vVariable[var][0].first,2)<<", ";
      for(int ind(0); ind<n-1; ind++) cout<<RoundNumber(cuts[ind],2)<<", ";
      cout<<RoundNumber(vVariable[var][vVariable[var].size()-1].first,2)<<endl<<"\t ";
      for(int ind(0); ind<n; ind++) cout<<RoundNumber(fractions[ind]*100,1)<<", ";
      cout<<endl<<endl;
    }
  } // Loop over 8 TeV and 13 TeV

   time(&curTime);
   cout<<endl<<"Sorting  "<<nentries<<" entries takes "<<difftime(curTime,startTime)<<" seconds"<<endl<<endl;  

}

