// efficiencies.C: Compares efficiencies of lepton pt, HT, and MET cuts

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

#define NVar 8

using namespace std;
using std::cout;
using std::endl;


void efficiencies(TString filetype = ".eps", bool abseff=true){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TString legNames[2][NHis] = {{"t#bar{t} [n=", "T1tttt(1150,525) [n=","T1tttt(1400,25) [n=","T1tttt(1400,1125) [n="},
			       {"t#bar{t} [n=", "T1tttt(1150,500) [n=","T1tttt(1500,1) [n=","T1tttt(1500,1155) [n="}}, legCaption;
  TChain *chain[2][NHis];
  ReadChains(chain);
  
  // Variables and cuts
  TString allcuts = "met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2";
  TString VarName[] = {"Max$(lep_pt*(abs(lep_id)==13))", "Max$(lep_pt*(abs(lep_id)==11))", "met", "ht",
		       "Max$(lep_pt*(abs(lep_id)==13))", "Max$(lep_pt*(abs(lep_id)==11))", "met", "ht"};
  TString Cuts[] = {"1", "1", "1", "1",
		    allcuts, allcuts, 
		    "Max$(lep_pt)>20&&ht>500&&njets[1]>=6&&nbm[1]>=2", 
		    "Max$(lep_pt)>20&&met>250&&njets[1]>=6&&nbm[1]>=2"};
  float Range[][2] = {{10, 80}, {10, 80}, {150, 500}, {250, 1000},
		      {10, 80}, {10, 80}, {150, 500}, {250, 1000}};
  int nBins[] = {35, 35, 35, 75,
		 35, 35, 35, 75};
  TString tags[] = {"", "", "", "", 
		    "allcuts", "allcuts", "allcuts", "allcuts"};

  // Histograms and canvas
  TCanvas can;
  TH1F *hFile[NVar][2][NHis];
  TH1F hEntries("hEntries","",100,0,5000);
  int colors[2][NHis] = {{kRed-7, kRed+1, kGreen+1, kMagenta+1}, {kBlue-7, kBlue+1, kGreen+2, kMagenta+2}};
  int fillStyles[2][NHis] = {{3345, 0, 0, 0}, {3354, 0, 0, 0}};
  TString xTitle = "", yTitle = "", Title = "", Pname, Hname, totCut, energies[] = {"_8_TeV", "_13_TeV"};
  TString logtag = "_log"; logtag += filetype;
  float maxHisto(-1), entries[2][NHis];
  double legX = 0.6, legY = 0.93;
  double legW = 0.12, legH = 0.22;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  TLine line; line.SetLineColor(2); line.SetLineWidth(2); line.SetLineStyle(2);
  TArrow arrow; arrow.SetLineWidth(2); arrow.SetArrowSize(.02);

  for(int var(0); var < NVar; var++){
    double xcut = -999.;
    xTitle = "";
    if(VarName[var].Contains("met")) {xTitle = "Cut on E_{T,miss}"; xcut = 250;}
    if(VarName[var].Contains("ht")) {xTitle = "Cut on H_{T}";  xcut = 500;}
    if(VarName[var].Contains("13")) {xTitle = "Cut on muon p_{T}"; xcut = 20;}
    if(VarName[var].Contains("11")) {xTitle = "Cut on electron p_{T}"; xcut = 20;}
    xTitle+=" (GeV)";
    yTitle = "Entries";
    leg.Clear(); leg.SetX1NDC(legX);leg.SetY2NDC(legY);
    for(int ene(0); ene < 2; ene++){
      Title = "Expected yields for 19.6 fb^{-1} at #sqrt{s} ="; Title += energies[ene]; 
      Title.ReplaceAll("_"," "); 
      for(int his(0); his < NHis; his++){
	//Creating histogram
	Hname = "histo"; Hname += var; Hname += ene; Hname += his;
	hFile[var][ene][his] = new TH1F(Hname, "", nBins[var], Range[var][0], Range[var][1]);
	hFile[var][ene][his]->SetLineColor(colors[ene][his]);
	hFile[var][ene][his]->SetLineWidth(3);
	hFile[var][ene][his]->SetFillStyle(fillStyles[ene][his]);
	hFile[var][ene][his]->SetFillColor(colors[ene][his]);
	hFile[var][ene][his]->SetXTitle(xTitle);
	hFile[var][ene][his]->SetYTitle(yTitle);
	hFile[var][ene][his]->SetTitle(Title);

	// Finding yields for different cuts
	totCut = "weight*(" + VarName[var] + ">";
	totCut += hFile[var][ene][his]->GetBinLowEdge(1); totCut += "&&";
	totCut += Cuts[var]; totCut += ")";
	//cout<<var<<", "<<ene<<", "<<his<<": "<<VarName[var]<<" and cut "<<totCut<<endl;
	chain[ene][his]->Project(Hname, VarName[var], totCut);
	for(int bin(1); bin <= nBins[var]; bin++){
	  hFile[var][ene][his]->SetBinContent(bin, hFile[var][ene][his]->Integral(bin, nBins[var]+1));
	}
	
	entries[ene][his] = hFile[var][ene][his]->Integral();

	if(maxHisto < hFile[var][ene][his]->GetMaximum()) maxHisto = hFile[var][ene][his]->GetMaximum();
      }
    } // Loop over energies

    for(int ene(0); ene < 2; ene++){
      leg.Clear();
      for(int his(0); his < NHis; his++){
	legCaption = legNames[ene][his]; legCaption += RoundNumber(entries[ene][his],0); legCaption += "]";
	leg.AddEntry(hFile[var][ene][his], legCaption);
	if(his==0) {
	  hFile[var][ene][his]->Draw();
	} else hFile[var][ene][his]->Draw("same");
      }
      leg.Draw();
      Pname = "plots/nolog/yields_"; Pname += var; Pname += tags[var]; Pname += energies[ene]; Pname += filetype;
      Pname.ReplaceAll("[","_"); Pname.ReplaceAll("]","");  Pname.ReplaceAll("+","-"); 
      can.SetLogy(0);    
      hFile[var][ene][0]->SetMaximum(maxHisto*1.2);
      can.SaveAs(Pname);

      can.SetLogy(1);
      hFile[var][ene][0]->SetMinimum(0.01);
      hFile[var][ene][0]->SetMaximum(maxHisto*4);
      arrow.SetLineColor(colors[ene][1]); 
      line.SetLineColor(colors[ene][1]); 
      line.DrawLine(xcut,0,xcut,maxHisto*2);
      arrow.DrawArrow(xcut,maxHisto,xcut+0.04*(Range[var][1]-Range[var][0]),maxHisto);
      if(VarName[var]=="nel+nmu"){
	line.DrawLine(2,0,2,maxHisto);
	arrow.DrawArrow(2,maxHisto,2-0.04*(Range[var][1]-Range[var][0]),maxHisto);
      }
      Pname.ReplaceAll(filetype, logtag); Pname.ReplaceAll("nolog/", "");
      can.SaveAs(Pname);
    } // Loop over energies

    // Normalizing histograms
    maxHisto = -1;
    for(int ene(0); ene < 2; ene++){
      for(int his(0); his < NHis; his++){
	if(his==1) continue;
	if(abseff){
	  chain[ene][his]->Project("hEntries", "ht", "weight");
	  hFile[var][ene][his]->Scale(100./hEntries.Integral());	  
	} else hFile[var][ene][his]->Scale(100./hFile[var][ene][his]->GetBinContent(1));
	if(maxHisto < hFile[var][ene][his]->GetMaximum()) maxHisto = hFile[var][ene][his]->GetMaximum();
      }
    } // Loop over energies

    leg.Clear(); 
    leg.SetY1NDC(legY-legH-0.04);
    leg.SetX1NDC(legX-0.04);
    for(int ene(0); ene < 2; ene++){
      for(int his(0); his < NHis; his++){
	if(his==1) continue;
	legCaption = legNames[ene][his]; legCaption += energies[ene];
	legCaption.ReplaceAll("_"," "); legCaption.ReplaceAll(" [n="," @"); 
	leg.AddEntry(hFile[var][ene][his], legCaption);
	if(ene==0 && his==0) {
	  hFile[var][ene][his]->SetTitle("");
	  if(abseff) hFile[var][ene][his]->SetYTitle("Total efficiency (%)");
	  else hFile[var][ene][his]->SetYTitle("Normalized efficiency (%)");
	  hFile[var][ene][his]->SetMaximum(maxHisto*1.1);
	  hFile[var][ene][his]->SetMinimum(0);
	  hFile[var][ene][his]->Draw();
	} else hFile[var][ene][his]->Draw("same");
      }
    } // Loop over energies
    leg.Draw();
    line.DrawLine(xcut,0,xcut,maxHisto*1.1);
    arrow.DrawArrow(xcut,maxHisto,xcut+0.04*(Range[var][1]-Range[var][0]),maxHisto);
    if(VarName[var]=="nel+nmu"){
      line.DrawLine(2,0,2,maxHisto);
      arrow.DrawArrow(2,maxHisto,2-0.04*(Range[var][1]-Range[var][0]),maxHisto);
    }
    Pname = "plots/nolog/efficiencies_"; Pname += var;  Pname += tags[var]; Pname += filetype;
    Pname.ReplaceAll("[","_"); Pname.ReplaceAll("]",""); Pname.ReplaceAll("+","-"); 
    can.SetLogy(0);    
    can.SaveAs(Pname);
    hFile[var][0][0]->SetMinimum(0.1);
    hFile[var][0][0]->SetMaximum(maxHisto*4);
    can.SetLogy(1);
    if(VarName[var]=="ht") hFile[var][0][0]->SetMaximum(maxHisto*15);
    Pname.ReplaceAll(filetype, logtag); Pname.ReplaceAll("nolog/", "");
    can.SaveAs(Pname);

  } // Loop over all variables
  for(int var(0); var < NVar; var++){
    for(int ene(0); ene < 2; ene++){
      for(int his(0); his < NHis; his++){
	if(hFile[var][ene][his]) hFile[var][ene][his]->Delete();
      }
    } // Loop over energies
  }
}

void TableYields(TString cuts, TChain *chain[2][4], TString tag1, TString tag2, ofstream &file, int iene){
  TString wcuts = "weight*("; wcuts += cuts; wcuts += ")";
  double yield[2][NHis], totalyield[2][NHis];
  TString Hname = "histo";
  TH1F *histo = new TH1F(Hname, "",100, 0, 10000);
  for(int ene(iene); ene < 2; ene++){
    for(int his(0); his < NHis; his++){
      chain[ene][his]->Project(Hname, "met", wcuts);
      yield[ene][his] = histo->Integral();
      chain[ene][his]->Project(Hname, "met", "weight");
      totalyield[ene][his] = histo->Integral();
    }
  }
  histo->Delete();
  TString samples[] = {"$t\\bar{t}(\\epsilon\\times1000)$", "T1ttt(1150,500)", 
		       "T1ttt(1500,0)  ", "T1ttt(1500,1150)"};
  file << "\n\\begin{tabular}{l | rrr";
  if(iene==0) file <<"| rrr";
  file <<"}\\hline\\hline\n"<<tag1;
  if(iene==0) file <<" & \\multicolumn{3}{c|}{\\bf{\\underline{8 TeV}}} "<<endl;
  file <<"& \\multicolumn{3}{c}{\\bf{\\underline{13 TeV}}} \\\\"<<endl<<tag2;
  if(iene==0) file <<" & \\multicolumn{1}{c}{$N$} & \\multicolumn{1}{c}{$\\epsilon (\\%)$} "
		   <<"& \\multicolumn{1}{c|}{$S/\\sqrt{B}$}"<<endl;
  file <<"& \\multicolumn{1}{c}{$N$} & \\multicolumn{1}{c}{$\\epsilon (\\%)$} "
       <<"& \\multicolumn{1}{c}{$S/\\sqrt{B}$} \\\\ \\hline"<<endl;
  for(int his(0); his < NHis; his++){
    file<<samples[his];
    for(int ene(iene); ene < 2; ene++){
      file<<" \t& "<< RoundNumber(yield[ene][his],0);
      if(his==0) {
	cout<<"tt "<<RoundNumber(yield[ene][his],0)<<" for "<<cuts<<endl;
	file<<" \t& "<< RoundNumber(yield[ene][his]*100000,1, totalyield[ene][his])<<" & -";
      } else file<<" \t& "<< RoundNumber(yield[ene][his]*100,1, totalyield[ene][his])<<" & "
		 << RoundNumber(yield[ene][his],2, sqrt(yield[ene][0]));
    }
    file<<" \\\\"<<endl;
  }
  file<< "\\hline\\hline\\end{tabular}"<<endl<<endl;
}

void YieldsPrint(){
  TChain *chain[2][NHis];
  ReadChains(chain);
  TString name = "txt/Yields_8_13_TeV.tex";
  ofstream file(name);

  TableYields("Max$(lep_pt)>20&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2",chain,"","RA4 cuts",file,0);
  TableYields("Max$(lep_pt)>165&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2",chain,"",
	      "RA4 + $p^\\ell_{T}>165$ GeV",file,1);
  TableYields("Max$(lep_pt)>20&&met>440&&ht>500&&njets[1]>=6&&nbm[1]>=2",chain,"",
   	      "RA4 + $E^\\text{miss}_{T}>440$ GeV",file,1);
  TableYields("Max$(lep_pt)>20&&met>250&&ht>1400&&njets[1]>=6&&nbm[1]>=2",chain,"","RA4 + $H_{T}>1400$ GeV",file,1);
  TableYields("Max$(lep_pt)>55&&met>350&&ht>900&&njets[1]>=6&&nbm[1]>=2",chain,"RA4 + $p^\\ell_{T}>55$ GeV",
	      "$E^\\text{miss}_{T}>350$ GeV, $H_{T}>900$ GeV",file,1);

  file.close();
  cout<<"Written "<<name<<endl;
}

