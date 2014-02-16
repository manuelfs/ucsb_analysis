// trigger_plot.C: Plots and fits with an error function trigger efficiency curves

#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"

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


#define NFiles 1

using namespace std;
using std::cout;
using std::endl;

Double_t errorFun(Double_t *x, Double_t *par) {
  return 0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[2]) )) ;
}

TGraphAsymmErrors CalcEffi(TH1F *hNum, TH1F *hDen) {
  TGraphAsymmErrors hEffi;
  hEffi.BayesDivide(hNum, hDen);

  float xhigh = hNum->GetBinLowEdge(hNum->GetNbinsX()+1);
  TF1 *fermiFunction = new TF1("fermiFunction",errorFun,0,xhigh,3);
  Double_t params[3] = {1.,200.,10.};            
  fermiFunction->SetParameters(params);
  fermiFunction->SetParNames("#epsilon","#mu","#sigma");
  fermiFunction->SetLineColor(4);
  fermiFunction->SetLineWidth(2);
  hEffi.Fit("fermiFunction","QR+");
  return hEffi;
}

TString ParseSampleName(TString file, TString &energy){
  TString sample = file;
  energy = file;
  if(sample.Contains("2012")){
    sample.Remove(0, sample.First("2012"));
    sample.Remove(sample.Index("-"), sample.Sizeof());
    sample.Insert(0,"Data ");
    energy = "8";
  } else {
    energy.Remove(energy.Index("TeV"), energy.Sizeof());
    energy.Remove(0, energy.Last('_')+1);
  }
  if(sample.Contains("TT")) sample = "t#bar{t}";
  if(sample.Contains("SMS")) sample = "T1ttt";

  return sample;
}

void trigger_plot(TString FileName){
  styles style; style.setDefaultStyle();
  gStyle->SetOptFit(1);
  gStyle->SetStatX(0.95);
  gStyle->SetStatY(0.35);
 
  TFile file(FileName);
  TCanvas can;
  TLatex label; label.SetTextSize(0.05); label.SetTextFont(22); label.SetTextAlign(21); label.SetNDC(true);
  TString xTitle, VarName, yTitle, Title, RefTrigger, PlotName, text, energy;
  int color = 1;
  TString sampleName = ParseSampleName(FileName, energy), sampleSimple;
  sampleSimple = sampleName; sampleSimple += "_"; sampleSimple += energy; sampleSimple += "TeV_";
  sampleSimple.ReplaceAll(" ", "_");
  sampleName += " @ "; sampleName += energy; sampleName += " TeV";

  //Loop over HT and MET 1D histograms 
  for(int obj(0); obj < file.GetListOfKeys()->GetSize(); ++obj){
    const std::string obj_name(file.GetListOfKeys()->At(obj)->GetName());
    VarName = obj_name;
    if(!VarName.Contains("Num_") || (!VarName.Contains("_HT_") && !VarName.Contains("_MET_"))) continue;

    TH1F  hNum = *(static_cast<TH1F*>(file.GetKey(VarName,1)->ReadObj()));
    VarName.ReplaceAll("Num_", "Den_");
    TH1F  hDen = *(static_cast<TH1F*>(file.GetKey(VarName,1)->ReadObj()));
				   
    TGraphAsymmErrors hEffi = CalcEffi(&hNum, &hDen);
    hEffi.SetLineColor(color);  hEffi.SetMarkerColor(color);
    hEffi.SetMarkerStyle(20); hEffi.SetMarkerSize(1.5);
    hEffi.SetLineWidth(2);

    Title=hNum.GetTitle();
    Title.Remove(0, Title.First("_")+1);
    Title.Remove(0, Title.First("_")+1);
    RefTrigger = Title;
    RefTrigger.Remove(RefTrigger.First("Vs")-1, RefTrigger.Sizeof()+1);
    Title.Remove(0, Title.First("Vs")+3);    
    Title.Insert(0,"#font[22]{");
    Title += "} #rightarrow Ref: #font[22]{"; Title += RefTrigger; Title += "}";
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
 
    VarName.ReplaceAll("Den_", "Eff_");
    
    PlotName = "plots/"; PlotName += sampleSimple; PlotName += VarName; PlotName += ".pdf";
    PlotName.ReplaceAll("#bar{t}","tbar");
    can.SetLogy(0);    
    can.SaveAs(PlotName);
  } //Loop over all variables

  //Loop over 2D efficiency histograms
  for(int obj(0); obj < file.GetListOfKeys()->GetSize(); ++obj){
    const std::string obj_name(file.GetListOfKeys()->At(obj)->GetName());
    VarName = obj_name;
    if(!VarName.Contains("Eff_HTMET_")) continue;

    TH2F  hEffi = *(static_cast<TH2F*>(file.GetKey(VarName,1)->ReadObj()));
    style.setHistoStyle(&hEffi);
    Title=hEffi.GetTitle();
    Title.Remove(0, Title.First("_")+1);
    Title.Remove(0, Title.First("_")+1);
    RefTrigger = Title;
    RefTrigger.Remove(RefTrigger.First("Vs")-1, RefTrigger.Sizeof()+1);
    Title.Remove(0, Title.First("Vs")+3);    
    Title.Insert(0,"#font[22]{");
    Title += "} #rightarrow Ref: #font[22]{"; Title += RefTrigger; Title += "}";
    hEffi.SetTitle(Title);
    hEffi.SetXTitle("H_{T} (GeV)"); 
    hEffi.SetYTitle("E_{T,miss} (GeV)"); 
    hEffi.SetMinimum(-0.01);
    hEffi.Draw("colz4");
    PlotName = "plots/"; PlotName += sampleSimple; PlotName += VarName; PlotName += ".pdf";
    PlotName.ReplaceAll("#bar{t}","tbar");
    can.SaveAs(PlotName);
  }
}
  
