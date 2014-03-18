// energy_compare.C: Compares different variables at different energies

#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "src/ucsb_utils.cpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include "TChain.h"
#include "TFile.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLatex.h"

#define NVar 3

using namespace std;
using std::cout;
using std::endl;

void energy_compare(){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TChain *chain[2][3];
  TString FileNames[2][3] = {{"archive/TT_CT10_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v2_AODSIM_UCSB1881_v71__f9.root",
			      "archive/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68_1150_525.root",
			      "archive/SMS-MadGraph_Pythia6Zstar_8TeV_T1tttt_2J_mGo-1100to1400_mLSP-525to1000_25GeVX25GeV_Binning_Summer12-START52_V9_FSIM-v2_AODSIM_UCSB1739reshuf_v68_1400_525.root"},
			     {"archive/TTbar_TuneZ2star_13TeV-powheg-tauola_Summer13dr53X-PU25bx25_START53_V19D-v1_AODSIM_UCSB2027_v71_.root",
			      "archive/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1145_500.root",
			      "archive/SMS-T1tttt_2J_mGo-845to3000_mLSP-1to1355_TuneZ2star_14TeV-madgraph-tauola_Summer12-START53_V7C_FSIM_PU_S12-v1_AODSIM_UCSB1949reshuf_v71_1500_500.root"}};
  TString legNames[2][3] = {{"t#bar{t} [n=", "T1tttt(1150,525) [n=","T1tttt(1400,525) [n="},
			    {"t#bar{t} [n=", "T1tttt(1150,500) [n=","T1tttt(1500,500) [n="}}, legCaption;
  

  // Variables and cuts
  TString VarName[] = {"met", "ht", "ntrupv"};
  TString Cuts[] = {"1", "1", "1/weight"};
  float Range[][2] = {{0,1000}, {0,2200}, {0,60}};
  int nBins[] = {50, 55, 60}, nHis = 3;

  // Histograms and canvas
  TCanvas can;
  TH1F *hFile[2][3];
  int colors[2][3] = {{kRed-7, kRed+1, kGreen+1}, {kBlue-7, kBlue+1, kGreen+2}};
  int fillStyles[2][3] = {{3345, 0, 0}, {3354, 0, 0}};
  TString xTitle = "", yTitle = "", Title = "", Pname, Hname, totCut, energies[] = {"_8_TeV", "_13_TeV"};;
  float maxHisto(-1), entries[2][3], means[2][3];
  double legX = 0.6, legY = 0.73;
  double legW = 0.12, legH = 0.2;
  TLegend leg(legX, legY, legX+legW, legY+legH);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  for(int var(0); var < NVar; var++){
    totCut = "weight*("; totCut += Cuts[var]; totCut += ")";

    xTitle = "";
    if(VarName[var]=="met") xTitle = "E_{T,miss}"; 
    if(VarName[var]=="ht") xTitle = "H_{T}"; 
    xTitle+=" (GeV)";
    if(VarName[var]=="ntrupv") xTitle = "Number of true PV"; 

    yTitle = "Entries ";
    if(VarName[var]=="met" || VarName[var]=="ht"){				   
      yTitle+="/(";
      yTitle+= RoundNumber((Range[var][1]-Range[var][0]), 0, (double)nBins[var]);
      yTitle+=" GeV)";
    }
    leg.Clear(); leg.SetX1NDC(legX);leg.SetY1NDC(legY);
    for(int ene(0); ene < 2; ene++){
      Title = "Expected yields for 19.6 fb^{-1} at #sqrt{s} ="; Title += energies[ene]; 
      Title.ReplaceAll("_"," "); 
      for(int his(0); his < nHis; his++){
	Hname = "histo"; Hname += ene; Hname += his;
	hFile[ene][his] = new TH1F(Hname, "", nBins[var], Range[var][0], Range[var][1]);
	hFile[ene][his]->SetLineColor(colors[ene][his]);
	hFile[ene][his]->SetLineWidth(3);
	hFile[ene][his]->SetFillStyle(fillStyles[ene][his]);
	hFile[ene][his]->SetFillColor(colors[ene][his]);
	hFile[ene][his]->SetXTitle(xTitle);
	hFile[ene][his]->SetYTitle(yTitle);
	hFile[ene][his]->SetTitle(Title);

	chain[ene][his] = new TChain("tree");
	chain[ene][his]->Add(FileNames[ene][his]);
	chain[ene][his]->Project(Hname, VarName[var], totCut);
	
	means[ene][his] = hFile[ene][his]->GetMean();
	double overflow = hFile[ene][his]->GetBinContent(nBins[var]+1);
	hFile[ene][his]->SetBinContent(nBins[var], hFile[ene][his]->GetBinContent(nBins[var])+overflow);
	entries[ene][his] = hFile[ene][his]->Integral();

	if(maxHisto < hFile[ene][his]->GetMaximum()) maxHisto = hFile[ene][his]->GetMaximum();
      }
    } // Loop over energies

    for(int ene(0); ene < 2; ene++){
      leg.Clear();
      for(int his(0); his < nHis; his++){
	legCaption = legNames[ene][his]; legCaption += RoundNumber(entries[ene][his],0); legCaption += "]";
	leg.AddEntry(hFile[ene][his], legCaption);
	if(his==0) {
	  hFile[ene][his]->Draw();
	} else hFile[ene][his]->Draw("same");
      }
      leg.Draw();
      Pname = "plots/yields_"; Pname += VarName[var]; Pname += energies[ene]; Pname += ".pdf";
      can.SetLogy(0);    
      hFile[ene][0]->SetMaximum(maxHisto*1.2);
      can.SaveAs(Pname);
      can.SetLogy(1);
      hFile[ene][0]->SetMinimum(0.01);
      hFile[ene][0]->SetMaximum(maxHisto*2);
      Pname.ReplaceAll(".pdf", "_log.pdf");
      can.SaveAs(Pname);
    } // Loop over energies

    maxHisto = -1;
    for(int ene(0); ene < 2; ene++){
      for(int his(0); his < 2; his++){
	hFile[ene][his]->Scale(1000./entries[ene][his]);
	if(maxHisto < hFile[ene][his]->GetMaximum()) maxHisto = hFile[ene][his]->GetMaximum();
      }
    } // Loop over energies

    leg.Clear(); leg.SetX1NDC(legX-0.02);leg.SetY1NDC(legY-0.04);
    for(int his(0); his < 2; his++){
      for(int ene(0); ene < 2; ene++){
	legCaption = legNames[ene][his]; legCaption += energies[ene];
	legCaption.ReplaceAll("_"," "); legCaption.ReplaceAll(" [n="," @"); 
	leg.AddEntry(hFile[ene][his], legCaption);
	if(ene==0 && his==0) {
	  hFile[ene][his]->SetTitle("Shape comparison");
	  hFile[ene][his]->Draw();
	} else hFile[ene][his]->Draw("same");
      }
    } // Loop over energies
    leg.Draw();
    Pname = "plots/shapes_"; Pname += VarName[var]; Pname += ".pdf";
    can.SetLogy(0);    
    hFile[0][0]->SetMaximum(maxHisto*1.2);
    if(VarName[var]=="ntrupv") hFile[0][0]->SetMaximum(100);
    can.SaveAs(Pname);
    can.SetLogy(1);
    hFile[0][0]->SetMinimum(0.01);
    hFile[0][0]->SetMaximum(maxHisto*2);
    if(VarName[var]=="ht") hFile[0][0]->SetMaximum(maxHisto*8);
    Pname.ReplaceAll(".pdf", "_log.pdf");
    can.SaveAs(Pname);

    for(int ene(0); ene < 2; ene++){
      for(int his(0); his < nHis; his++){
	hFile[ene][his]->Delete();
      }
    } // Loop over energies
  } // Loop over all variables
}
    
