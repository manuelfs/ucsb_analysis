// compare_plots.C: Plots on the same canvas all histograms with the same name in various files

#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "src/ucsb_utils.cpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include "TFile.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TKey.h"
#include "TList.h"
#include "TString.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLatex.h"

#define NFiles 2

using namespace std;
using std::cout;
using std::endl;

void compare_plots(TString plotTag = "Eff_HT_", bool doScale=false){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TString FileNames[] = {"Archive/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68_1150_800.root",
			 "Archive/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1145_800.root"};
  
  TString legNames[] = {"T1tttt(1150,800) @ 8 TeV ","T1tttt(1145,800) @ 14 TeV ", "T1tttt(1145,500) @ 14 TeV ",
			"ttbar_ll_8TeV", "ttbar_hh_8TeV", "t#bar{t} @ 8 TeV ", "t#bar{t} @ 13 TeV "}, Pname;
  TFile *histosFile[NFiles];
  vector<TString> VarName;
  vector<int> iVarName;
  for(int iFiles(0); iFiles < NFiles; iFiles++){
    histosFile[iFiles] = new TFile(FileNames[iFiles]);
    for(int obj(0); obj < histosFile[iFiles]->GetListOfKeys()->GetSize(); ++obj){
      TString obj_name(histosFile[iFiles]->GetListOfKeys()->At(obj)->GetName());
      if(iFiles==0) {
	VarName.push_back(obj_name);
	iVarName.push_back(1);
      } else {
	for(unsigned int var(0); var < VarName.size(); var++) {
	  if(VarName[var] == obj_name) {
	    iVarName[var]++;
	    break;
	  }
	} // Loop over stored file 0 variables for other files
      }
    } // Loop over file 0 variables
  }
  
  int colors[] = {2, 4, kSpring-9, kSpring-9, kSpring-9, kGreen+2, kBlue+1};
  int fillStyles] = {0, 0, 3345, 3345, 3345, 0, 3354, };

  ofstream texFile; texFile.open("txt/Averages.tex");

  TH1F hFile[NFiles];
  TCanvas can;
  //Loop over all variables  
  TString xTitle = "", texName;
  TString yTitle = "";
  TString Title = "";
  for(unsigned int var(0); var < VarName.size(); var++) {
    if(iVarName[var] < NFiles || !VarName[var].Contains(plotTag)) continue;

    xTitle = "";
    texName = "";
    if(VarName[var].Contains("_HT_")) {xTitle = "H_{T} "; texName = "$\\bf{\\left<H_T \\right>} \\mathbf{(GeV)}$";};
    if(VarName[var].Contains("_MET_")) {xTitle = "E_{T,miss} ";  texName = "$\\bf{\\left<E_{T,miss} \\right>} \\mathbf{(GeV)}$";};
    xTitle+="(GeV)";
				   
    double maxhisto(0), means[NFiles];  
    TLegend leg(0.52,0.12,0.97,0.35);
    leg.SetTextSize(0.055); leg.SetFillColor(0); leg.SetBorderSize(0);
    leg.SetTextFont(132); leg.SetFillStyle(0);
    for(int iFiles(0); iFiles < NFiles; iFiles++){
      hFile[iFiles] = *(static_cast<TH1F*>(histosFile[iFiles]->GetKey(VarName[var],1)->ReadObj()));
      style.styleHist(&hFile[iFiles], colors[iFiles], fillStylesiFiles], 20, 1.4, 2);
      Title=hFile[iFiles].GetTitle();
      if(VarName[var].Contains("ets")){ 
	Title+=", p_{T}^{thresh} = ";
	TString VarName2 = VarName[var];
	VarName2.Remove(0, VarName2.Sizeof()-3);
	Title+=VarName2;
	Title+= " GeV";
      }    
      yTitle = "Entries ";
      if(VarName[var].Contains("HT")||VarName[var].Contains("MET")||VarName[var].Contains("MT")||VarName[var].Contains("pTVeto")||VarName[var].Contains("pTRA4")||VarName[var].Contains("pT_")){				   
	yTitle+="/ (";
	yTitle+= RoundNumber(hFile[iFiles].GetBinWidth(1), 0);
	yTitle+=" GeV)";
      }
      hFile[iFiles].SetTitle(Title);
      hFile[iFiles].SetXTitle(xTitle);
      hFile[iFiles].SetYTitle(yTitle);
      //hFile[iFiles].SetTextSize(0.06);            // Set global text size
      hFile[iFiles].SetTitleSize(0.05,"xy");     // Set the 2 axes title size
      hFile[iFiles].SetLabelSize(0.05,"xy");     // Set the 2 axes label size
      if(doScale) hFile[iFiles].Scale(1000./hFile[iFiles].Integral());    
      //cout<<"The Mean of "<< tagNames[iFiles]<<" is "<<hFile[iFiles].GetMean()<<endl;
      if(hFile[iFiles].GetMaximum() > maxhisto) maxhisto = hFile[iFiles].GetMaximum();
      leg.AddEntry(&hFile[iFiles], legNames[iFiles]);
      means[iFiles] = hFile[iFiles].GetMean();
    } //Loop over all files
    //cout<<"Writing table"<<endl;   
    int digits = 1;
    if(VarName[var].Contains("Iso")) digits = 3;
    texFile << VarName[var] << endl;
    texFile << "\\begin{tabular}{c | ccc}\n \\hline\\hline\n"<<texName<<" & 8 TeV & 13/14 TeV & $\\Delta$ (\\%) \\\\"<<endl;
    texFile << "\\hline\n $t\\bar{t}$ & "<< RoundNumber(means[2],digits) << " & " << RoundNumber(means[3],digits) << " & "<<
      RoundNumber((means[3]-means[2])*100, 1, means[2]) << "\\\\" << endl;
    texFile << "T1tttt & "<< RoundNumber(means[4],digits) << " & " << RoundNumber(means[5],digits) << " & "<<
      RoundNumber((means[5]-means[4])*100, 1, means[4]) << "\\\\ \\hline\\hline" << endl;
    texFile << "\\end{tabular}"<<endl<<endl;
    for(int iFiles(0); iFiles < NFiles; iFiles++){
      if(iFiles==0){
	hFile[iFiles].SetMaximum(1.2*maxhisto); 
	hFile[iFiles].Draw("");
      }else hFile[iFiles].Draw("same");
    } //Loop over all files
    leg.Draw();
    Pname = "plots/"; Pname += VarName[var]; Pname += ".pdf";
    can.SetLogy(0);    
    can.SaveAs(Pname);
//     hFile[0].SetMaximum(20*maxhisto);    
//     if(VarName[var].Contains("PU")) hFile[2].SetMaximum(100); 
//     can.SetLogy(1);
//     hFile[0].SetMinimum(0.05);
//     Pname.ReplaceAll(".pdf", "_log.pdf");
//     can.SaveAs(Pname);
  } //Loop over all variables

  texFile.close();
  for(int iFiles(0); iFiles < NFiles; iFiles++){
    histosFile[iFiles]->Close();
    histosFile[iFiles]->Delete();
  }
}
    
