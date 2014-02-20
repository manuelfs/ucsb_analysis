// trigger_plot.C: Plots and fits with an error function trigger efficiency curves

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
#include "TF1.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TKey.h"
#include "TList.h"
#include "TString.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"

#define NFiles 4

using namespace std;
using std::cout;
using std::endl;

Double_t errorFun(Double_t *x, Double_t *par) {
  return 0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[2]) )) ;
}

TGraphAsymmErrors CalcEffi(TH1F *hNum, TH1F *hDen, TString name, bool doFit=true) {
  TGraphAsymmErrors hEffi;
  hEffi.SetName(name);
  hEffi.BayesDivide(hNum, hDen);

  float xhigh = hNum->GetBinLowEdge(hNum->GetNbinsX()+1);
  TF1 *fermiFunction = new TF1("fermiFunction",errorFun,0,xhigh,3);
  Double_t params[3] = {1.,200.,10.};    
  if(doFit){
    fermiFunction->SetParameters(params);
    fermiFunction->SetParNames("#epsilon","#mu","#sigma");
    fermiFunction->SetLineColor(4);
    fermiFunction->SetLineWidth(2);
    hEffi.Fit("fermiFunction","QR+");
  }
  return hEffi;
}

void makeTitle(TString &Title){
  Title.Remove(0, Title.First("_")+1);
  Title.Remove(0, Title.First("_")+1);
  TString RefTrigger = Title;
  RefTrigger.Remove(RefTrigger.First("Vs")-1, RefTrigger.Sizeof()+1);
  Title.Remove(0, Title.First("Vs")+3);    
  Title.Insert(0,"#font[22]{");
  Title += "} #rightarrow Ref: #font[22]{"; Title += RefTrigger; Title += "}";
}

void compare_efficiency(){
  styles style("Standard"); style.setDefaultStyle();
 
  //Files
  TString FileNames[] = {"Archive/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68_1150_800.root",
			 "Archive/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1145_800.root",
			 "Archive/SingleMu_Run2012B-13Jul2012-v1_AOD_UCSB1554ra4_v66_.root",
			 "Archive/SingleMu_Run2012D-PromptReco-v1_AOD_UCSB1628ra4_v67_.root"};
  
  TString legNames[] = {"T1tttt(1150,800) @ 8 TeV ","T1tttt(1145,800) @ 14 TeV ", 
			"Data 2012B @ 8 TeV ", "Data 2012D @ 8 TeV", "T1tttt(1145,500) @ 14 TeV ",
			"ttbar_ll_8TeV", "ttbar_hh_8TeV", "t#bar{t} @ 8 TeV ", "t#bar{t} @ 13 TeV "}, Pname;
  TFile *histosFile[NFiles];
  vector<TString> VarName[NFiles];
  vector<int> iVarName;
  for(int iFiles(0); iFiles < NFiles; iFiles++){
    histosFile[iFiles] = new TFile(FileNames[iFiles]);
    for(int obj(0); obj < histosFile[iFiles]->GetListOfKeys()->GetSize(); ++obj){
      TString obj_name(histosFile[iFiles]->GetListOfKeys()->At(obj)->GetName());
      if(iFiles==0) {
	for(int ifil(0); ifil < NFiles; ifil++)
	  VarName[ifil].push_back(obj_name);
	iVarName.push_back(1);
      } else {
	TString clean_obj_name = obj_name;
	clean_obj_name.ReplaceAll("_eta2p1","");
	clean_obj_name.ReplaceAll("PFNoPU","PF");
	for(unsigned int var(0); var < VarName[0].size(); var++) {
	  TString cleanVarName0 = VarName[0][var];
	  cleanVarName0.ReplaceAll("_eta2p1","");
	  cleanVarName0.ReplaceAll("PFNoPU","PF");
	  if(cleanVarName0 == clean_obj_name) {
	    VarName[iFiles][var] = obj_name;
	    iVarName[var]++;
	    break;
	  }
	} // Loop over stored file 0 variables for other files
      }
    } // Loop over file 0 variables
  }
  
  int colors[] = {2, 4, 1, 8};

  TGraphAsymmErrors hEffi[NFiles];
  TCanvas can;
  TString xTitle = "", yTitle = "", Title = "";

  //Loop over all common variables  
  for(unsigned int var(0); var < VarName[0].size(); var++) {
    if(iVarName[var] < NFiles || (!VarName[0][var].Contains("Num_HT_") && !VarName[0][var].Contains("Num_MET_"))) continue;
    TLegend leg(0.45,0.14,0.97,0.39);
    leg.SetTextSize(0.052); leg.SetFillColor(0); leg.SetBorderSize(0);
    leg.SetTextFont(132); leg.SetFillStyle(0);
    for(int iFiles(0); iFiles < NFiles; iFiles++){
      TH1F  hNum = *(static_cast<TH1F*>(histosFile[iFiles]->GetKey(VarName[iFiles][var],1)->ReadObj()));
      VarName[iFiles][var].ReplaceAll("Num_", "Den_");
      TH1F  hDen = *(static_cast<TH1F*>(histosFile[iFiles]->GetKey(VarName[iFiles][var],1)->ReadObj()));
      VarName[iFiles][var].ReplaceAll("Den_", "Num_");

      Title = "Effi_"; Title+=var; Title+=iFiles;
      hEffi[iFiles] = CalcEffi(&hNum, &hDen, Title, false);
      hEffi[iFiles].SetLineColor(colors[iFiles]);  hEffi[iFiles].SetMarkerColor(colors[iFiles]);
      hEffi[iFiles].SetMarkerStyle(20); hEffi[iFiles].SetMarkerSize(1.5);
      hEffi[iFiles].SetLineWidth(2);
      Title=hNum.GetTitle();
      makeTitle(Title);
      yTitle = "Trigger efficiency";
      xTitle = ""; 
      if(VarName[iFiles][var].Contains("_HT_")) {
	xTitle = "H_{T}"; 
	if(legNames[iFiles].Contains("2012D")) legNames[iFiles] = "Data 2012D @ 8 TeV (NoPUHT)";
      }
      if(VarName[iFiles][var].Contains("_MET_")) {
	xTitle = "E_{T,miss}";
 	if(legNames[iFiles].Contains("2012D")) legNames[iFiles] = "Data 2012D @ 8 TeV";
      }
     xTitle+=" (GeV)";
      hEffi[iFiles].SetTitle(Title);
      hEffi[iFiles].GetXaxis()->SetTitle(xTitle);
      hEffi[iFiles].GetYaxis()->SetTitle(yTitle);
      hEffi[iFiles].SetFillStyle(0);
      leg.AddEntry(&hEffi[iFiles], legNames[iFiles]);
    } //Loop over all files
    for(int iFiles(0); iFiles < NFiles; iFiles++){
      if(iFiles==0){
	hEffi[iFiles].SetMaximum(1.1); 
	hEffi[iFiles].Draw("AP");
      }else hEffi[iFiles].Draw("P same");
    } //Loop over all files
    leg.Draw();
    Pname = "plots/CompEfficiency_"; Pname += VarName[0][var]; Pname += ".pdf";
    can.SaveAs(Pname);
  } // Loop over all common variables  

  for(int iFiles(0); iFiles < NFiles; iFiles++){
    histosFile[iFiles]->Close();
    histosFile[iFiles]->Delete();
  }
}
    

void trigger_plot(TString FileName, bool is2D = false){
  styles style("Standard"); 
  if(is2D) style.setGroup("2D");
  style.setDefaultStyle();
  gStyle->SetOptFit(1);
  gStyle->SetStatX(0.95);
  gStyle->SetStatY(0.35);
 
  TFile file(FileName);
  TGraphAsymmErrors hEffi; TH2F hEffi2D;
  TCanvas can;
  int color = 1;
  TLatex label; label.SetTextSize(0.05); label.SetTextFont(22); label.SetTextAlign(21); label.SetNDC(true);
  TString xTitle, VarName, yTitle, Title, PlotName, text, sampleSimple;
  TString sampleName = GetSampleEnergy(FileName, sampleSimple);

  //Loop over HT and MET 1D or Efficiency 2D histograms
  for(int obj(0); obj < file.GetListOfKeys()->GetSize(); ++obj){
    VarName = file.GetListOfKeys()->At(obj)->GetName();
    if(is2D){
      if(!VarName.Contains("Eff_HTMET_")) continue;

      hEffi2D = *(static_cast<TH2F*>(file.GetKey(VarName,1)->ReadObj()));
      style.setHistoStyle(&hEffi2D);
      Title=hEffi2D.GetTitle();
      makeTitle(Title);
      hEffi2D.SetTitle(Title);
      hEffi2D.SetXTitle("H_{T} (GeV)"); 
      hEffi2D.SetYTitle("E_{T,miss} (GeV)"); 
      hEffi2D.SetMinimum(-0.01);
      hEffi2D.Draw("colz4");
    } else {
      if(!VarName.Contains("Num_") || (!VarName.Contains("_HT_") && !VarName.Contains("_MET_"))) continue;

      TH1F  hNum = *(static_cast<TH1F*>(file.GetKey(VarName,1)->ReadObj()));
      VarName.ReplaceAll("Num_", "Den_");
      TH1F  hDen = *(static_cast<TH1F*>(file.GetKey(VarName,1)->ReadObj()));
      VarName.ReplaceAll("Den_", "Eff_");
				   
      Title = "Effi_"; Title+=obj;
      hEffi = CalcEffi(&hNum, &hDen, Title);
      hEffi.SetLineColor(color);  hEffi.SetMarkerColor(color);
      hEffi.SetMarkerStyle(20); hEffi.SetMarkerSize(1.5);
      hEffi.SetLineWidth(2);
      Title=hNum.GetTitle();
      makeTitle(Title);
      yTitle = "Trigger efficiency";
      xTitle = ""; 
      if(VarName.Contains("HT")) xTitle = "H_{T}"; 
      if(VarName.Contains("MET")) xTitle = "E_{T,miss}";
      xTitle+=" (GeV)";
      hEffi.SetTitle(Title);
      hEffi.GetXaxis()->SetTitle(xTitle);
      hEffi.GetYaxis()->SetTitle(yTitle);
      //hEffi.GetXaxis()->SetRange(5, hEffi.GetXaxis()->GetNbins()-4);   
      double maxhisto(0);  
      if(hEffi.GetMaximum() > maxhisto) maxhisto = hEffi.GetMaximum();
      hEffi.Draw("AP");
      label.DrawLatex(0.78, 0.38, sampleName);
    }
    PlotName = "plots/"; PlotName += sampleSimple; PlotName += VarName; PlotName += ".pdf";
    PlotName.ReplaceAll("#bar{t}","tbar");
    can.SaveAs(PlotName);
  } //Loop over all variables
}
  
