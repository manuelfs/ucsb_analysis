// reweight.C: Calculates PU reweighting

#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "src/ucsb_utils.cpp"

#include <fstream>
#include <iostream>
#include "TChain.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1F.h"

using namespace std;
using std::cout;
using std::endl;

void reweight(bool doPlot=false){
  styles style("Standard"); style.setDefaultStyle();
  TCanvas can;

  TString filename[] = {"root/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1145_800.root",
			"root/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68_1150_800.root"};
  TChain cDen("tree"), cNum("tree");
  cDen.Add(filename[0]); cNum.Add(filename[1]); 

  double binning[] = {0,11,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,38,42,60};
  int nBins(28);
  TString sDen("hDen"), sNum("hNum");
  TH1F hDen(sDen, sDen, nBins, binning); TH1F hNum(sNum, sNum, nBins, binning);
  cDen.Project(sDen,"ntrupv");
  cNum.Project(sNum,"ntrupv");

  TString textname = "txt/weights_sms_pu.txt";
  ofstream fileweights(textname);
  fileweights<<"min\tmax\t  weight"<<endl;
  for(int bin(0); bin < nBins; bin++){
    fileweights<<binning[bin]<<"\t"<<binning[bin+1]<<"\t  ";
    fileweights<<RoundNumber(hDen.GetBinContent(bin+1),4,hNum.GetBinContent(bin+1))<<endl;
  }
  fileweights.close();
  cout<<"PU weights saved in "<<textname<<endl;

  if(doPlot){
    hNum.SetLineColor(2);
    hNum.Scale(hDen.Integral()/hNum.Integral());  
    hDen.SetMinimum(0);
    hDen.Draw();
    hNum.Draw("same");
    can.SaveAs("plots/pu_plot.pdf");
  }
}
