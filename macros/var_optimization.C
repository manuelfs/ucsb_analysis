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

void divide_var(vector<float> &cuts, vector<float> &fractions, int nbins, vector<double_double> variable, double totw){
  double fbin = 1./(double)nbins, sumw(0), fw, prev_fw(0);
  double desired_fw = fbin;
  int bin = 1;
  for(unsigned int entry(0); entry<variable.size(); entry++){
    sumw += variable[entry].second;
    fw = sumw/totw;
    //cout<<entry<<": fw "<<fw<<", desired fw "<<desired_fw<<endl;
    while(fw >= desired_fw) {
      fractions.push_back(fw);
      for(int ibin(0); ibin<bin-1; ibin++) fractions[bin-1] -= fractions[ibin];
      if(fw-desired_fw < desired_fw-prev_fw) cuts.push_back(variable[entry].first);
      else {
	cuts.push_back(variable[entry-1].first);
	fractions[bin-1] -= variable[entry].second/totw;
      }
      //cout<<"Pushed entry "<<bin<<" with cut "<<cuts[bin-1]<<" and fraction "<<fractions[bin-1]<<endl;
      bin++;
      if(bin==nbins) break;
      desired_fw = fbin*(double)bin;
    }
    if(bin==nbins) break;
    prev_fw = fw;
  }
  cuts.push_back(variable[variable.size()-1].first*10);
  fractions.push_back(1);
  int last = fractions.size()-1;
  for(int ibin(0); ibin<last; ibin++) fractions[last] -= fractions[ibin];  
}

void var_optimization(long fixnentries=1000, int nDivisions = 10){
  TChain *allchains[2][NHis], *chain;
  ReadChains(allchains);
  long nentries;

  float keep_greater[] = {1, 1, -1, 1}; // Indicates whether we should keep <cut or >cut
  TString sVariable[] = {"spher", "mt", "dr_bb", "dphi_wlep"};
  vector<double_double> vVariable[NVar];
  float fVariable[NVar];
  double totYield[2][NHis];
  float weight, ht, met;
  int nlep;
  vector<int> *njets(0), *nbm(0);
  vector<float> Yields[2][NHis][NVar], cutLimits[NVar];

  time_t startTime, curTime;
  time(&startTime);
   for(int ene(0); ene < 1; ene++){
     for(int his(NHis-1); his >= 1; his--){
       bool isFirst = (ene==0 && his == NHis-1);

       // Setting up chain addresses
       chain = allchains[ene][his];
       if(chain->GetEntries() < fixnentries || fixnentries < 0) nentries = chain->GetEntries();
       else nentries = fixnentries;
       chain->SetBranchAddress("weight", &weight);
       chain->SetBranchAddress("nlep", &nlep);
       chain->SetBranchAddress("ht", &ht);
       chain->SetBranchAddress("met", &met);
       chain->SetBranchAddress("nbm", &nbm);
       chain->SetBranchAddress("njets", &njets);
       for(int var(0); var < NVar; var++){
 	chain->SetBranchAddress(sVariable[var], &fVariable[var]);
 	vVariable[var].resize(0);
 	for(int cut(0); cut < nDivisions; cut++)
 	  Yields[ene][his][var].push_back(0);
       }
       totYield[ene][his] = 0;
       cout<<endl<<ene<<", "<<his<<": Doing "<<nentries<<" entries"<<endl;
       for(int entry(0); entry<nentries; entry++){
 	chain->GetEntry(entry);
 	weight = 1;
 	totYield[ene][his] += weight;
	if(nlep!=1 || met<=250 || ht<= 500 || njets->at(1)<6 || nbm->at(1)<2) continue; // RA4 cuts 
 	for(int var(0); var < NVar; var++){
 	  Yields[ene][his][var][0] += weight;
 	  if(isFirst){ // Finding cut limits with first chain
 	    vVariable[var].push_back(make_pair(fVariable[var], weight));
 	  } else {
  	    for(unsigned int cut(0); cut < cutLimits[var].size()-1; cut++){
	      int icut = cut;
	      if(keep_greater[var] < 0) icut = cutLimits[var].size()-2-cut;
  	      if(fVariable[var]*keep_greater[var] > cutLimits[var][icut]*keep_greater[var]) 
  		Yields[ene][his][var][cut+1] += weight;
	    }
 	  }
 	} // Loop over variables
       } // Loop over all entries

       if(isFirst){ // Sorting and finding ranges
 	for(int var(0); var < NVar; var++){
 	  cout<<var<<": Tot yield "<<totYield[ene][his]<<", after cut "<<Yields[ene][his][var][0]
 	      <<", entries in vVariable "<<vVariable[var].size()<<endl;
 	  if(vVariable[var].size()==0){cout<<"No events passed cuts"<<endl; return;}
 	//       cout<<endl<<sVariable[var]<<" Before sorting: "<<vVariable[var][0].first<<", "<<vVariable[var][1].first<<", "
 	  // 	  <<vVariable[var][vVariable[var].size()-1].first<<endl;
 	  sort(vVariable[var].begin(), vVariable[var].end(), dd_small2big);
 	  vector<float> fractions;
 	  divide_var(cutLimits[var], fractions, nDivisions, vVariable[var], Yields[ene][his][var][0]);
    	  cout<<cutLimits[var].size()<<" bins "<<sVariable[var] <<": "<<RoundNumber(vVariable[var][0].first,2)<<", ";
    	  for(uint ind(0); ind<cutLimits[var].size()-1; ind++) cout<<RoundNumber(cutLimits[var][ind],2)<<", ";
    	  cout<<RoundNumber(vVariable[var][vVariable[var].size()-1].first,2)<<endl;
//     	  for(int ind(0); ind<nDivisions; ind++) cout<<RoundNumber(fractions[ind]*100,1)<<", ";
//     	  cout<<endl;
 	  for(unsigned int row(1); row < cutLimits[var].size(); row++)
 	    for(unsigned int col(0); col < cutLimits[var].size(); col++){
 	      int icol(col), irow(row);
 	      if(keep_greater[var]<0) icol = cutLimits[var].size()-col-1;
 	      if(icol >= irow) 
 		Yields[ene][his][var][row] += Yields[ene][his][var][0]*fractions[col];
 	    }
	  cout<<endl;
 	} // Loop over variables
       } // If it is first iteration
        for(int var(0); var < NVar; var++){
  	cout<<ene<<", "<<his<<": Yields -> ";
  	for(unsigned int cut(0); cut < cutLimits[var].size(); cut++)
  	  cout<<RoundNumber(Yields[ene][his][var][cut], 1)<<", ";
  	cout<<endl;
        }
     } // Loop over samples (NHis)
   } // Loop over 8 TeV and 13 TeV

  time(&curTime);
  cout<<endl<<"Sorting  "<<nentries<<" entries takes "<<difftime(curTime,startTime)<<" seconds"<<endl<<endl;  

}

