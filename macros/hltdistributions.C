#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "inc/ucsb_utils.hpp"
#include "src/ucsb_utils.cpp"

#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"

//
//TH1F initialization
//
TH1F* InitTH1F(char* Name, char* Title, int Nbins, double XMin, double XMax){
  TH1F *h1 = new TH1F(Name, Title, Nbins, XMin, XMax);
  h1->Sumw2();
  return h1;
}

//
// h1 cosmetics
//
void h1cosmetic(TH1F* &h1, char* title, int linecolor=kRed, int linewidth=1, int fillcolor=0, TString var=""){
  h1->SetLineColor(linecolor);
  h1->SetLineWidth(linewidth);
  h1->SetFillColor(fillcolor);
  h1->SetTitle(title);
  h1->SetName(title);
  h1->SetXTitle(var);
  h1->SetYTitle("a.u.");
  h1->SetStats(0);
  h1->SetMinimum(0.1);
}

void Draw1D(TString var, bool isMu, int Nbins, float BeginRange, float EndRange) { 
  TString dir_711, dir_710;
  if(isMu)  dir_711 = "/homes/manuelf/code/ucsb_analysis/root/hlt/conf711/mu15/"; 
  if(!isMu)  dir_711 = "/homes/manuelf/code/ucsb_analysis/root/hlt/conf711/el15/"; 
  if(isMu)  dir_710 = "/homes/manuelf/code/ucsb_analysis/root/hlt/conf710/mu15/"; 
  if(!isMu)  dir_710 = "/homes/manuelf/code/ucsb_analysis/root/hlt/conf710/el15/"; 
  /*
   *Br    2 :onmet     : onmet/F                                                *
   *Br    3 :onht      : onht/F                                                 *
   *Br    4 :weight    : weight/F                                               *
   *Br    5 :wl1ht200  : wl1ht200/F                                             *
   *Br    6 :genht     : genht/F                                                *
   */
  TChain *ch_qcd_711 = new TChain("tree"); 
  ch_qcd_711->Add(dir_711+"QCD*");
  TChain *ch_1025_711 = new TChain("tree"); 
  ch_1025_711->Add(dir_711+"T1tttt_1025_625*.root");
  TChain *ch_qcd_710 = new TChain("tree"); 
  ch_qcd_710->Add(dir_710+"QCD*");
  TChain *ch_1025_710 = new TChain("tree"); 
  ch_1025_710->Add(dir_710+"T1tttt_1025_625*.root");

  TH1F *h1_qcd_711    = InitTH1F("h1_qcd_711", "h1_qcd_711",  Nbins, BeginRange, EndRange); 
  TH1F *h1_1025_711   = InitTH1F("h1_1025_711", "h1_1025_711", Nbins, BeginRange, EndRange); 
  TH1F *h1_qcd_710    = InitTH1F("h1_qcd_710", "h1_qcd_710",  Nbins, BeginRange, EndRange); 
  TH1F *h1_1025_710   = InitTH1F("h1_1025_710", "h1_1025_710", Nbins, BeginRange, EndRange); 
  
  ch_qcd_711->Draw(Form("%s>>h1_qcd_711",var.Data()), "weight*0.14/19600", "goff");
  ch_1025_711->Draw(Form("%s>>h1_1025_711",var.Data()), "weight*0.14/19600", "goff");
  ch_qcd_710->Draw(Form("%s>>h1_qcd_710",var.Data()), "weight*0.14/19600", "goff");
  ch_1025_710->Draw(Form("%s>>h1_1025_710",var.Data()), "weight*0.14/19600", "goff");

  TString XTitle; 
  if(var=="onht") XTitle = "HLT HT (GeV)";
  if(var=="onmet") XTitle = "HLT MET (GeV)";
  if(var=="(genht-onht)/genht") XTitle = "#frac{gen HT  - HLT HT}{gen HT}";

  //h1cosmetic(TH1F* &h1, char* title, int linecolor=kRed, int linewidth=1, int fillcolor=0, TString var="")
  h1cosmetic(h1_qcd_711, "", kRed, 2, 0, XTitle);
  h1cosmetic(h1_1025_711, "1025_711", kBlue, 2, 0, XTitle);
  h1cosmetic(h1_qcd_710, "", kRed, 4, 0, XTitle);
  h1cosmetic(h1_1025_710, "1025_710", kBlue, 4, 0, XTitle);
  h1_qcd_710->SetLineStyle(4); 
  h1_1025_710->SetLineStyle(4); 

  h1_qcd_711->Scale(1./h1_qcd_711->Integral());
  h1_1025_711->Scale(1./h1_1025_711->Integral());
  h1_qcd_710->Scale(1./h1_qcd_710->Integral());
  h1_1025_710->Scale(1./h1_1025_710->Integral());


  TCanvas *c = new TCanvas();
  c->cd(1);
  h1_qcd_711->SetMaximum(h1_qcd_711->GetMaximum()*1.7);
  if(var=="(genht-onht)/genht") {
    h1_qcd_711->GetXaxis()->SetTitleOffset(1.2);
    c->cd(1)->SetBottomMargin(0.15);
  }  
  h1_qcd_711->Draw("hist");
  h1_1025_711->Draw("hist same");
  h1_qcd_710->Draw("hist same");
  h1_1025_710->Draw("hist same");

  double legX = 0.4, legY = 0.93;
  double legW = 0.35, legH = 0.25;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  leg.SetFillColor(kWhite);
  leg.SetLineColor(kWhite);
  leg.SetShadowColor(kWhite);
  leg.AddEntry(h1_qcd_711,    Form(" 711 QCD [mean = %.2f]", h1_qcd_711->GetMean()),                 "l");
  leg.AddEntry(h1_qcd_710,    Form(" 710 QCD [mean = %.2f]", h1_qcd_710->GetMean()),                 "l");
  leg.AddEntry(h1_1025_711,   Form(" 711 T1tttt(1025,625) [mean = %.2f]", h1_1025_711->GetMean()),   "l");
  leg.AddEntry(h1_1025_710,   Form(" 710 T1tttt(1025,625) [mean = %.2f]", h1_1025_710->GetMean()),   "l");
  leg.Draw();


  TString FileName = var;  
  if(var=="(genht-onht)/genht") FileName = "HTres";
  c->Print(Form("plots/hltstudy/dist_%s_%s.eps", FileName.Data(), isMu?"M":"E"));

}

void hltdistributions() { 
  styles style("Standard"); style.setDefaultStyle();

  Draw1D("onht",false,50,0,2000);
  Draw1D("onht",true,50,0,2000);
  Draw1D("onmet",false,50,0,500);
  Draw1D("onmet",true,50,0,500);
  Draw1D("(genht-onht)/genht",false,50,-1,1);
  Draw1D("(genht-onht)/genht",true,50,-1,1);

}
