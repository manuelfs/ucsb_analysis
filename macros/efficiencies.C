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


void efficiencies(TString filetype = ".eps"){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TString legNames[2][NHis] = {{"t#bar{t} [n=", "T1tttt(1150,525) [n=","T1tttt(1400,25) [n=","T1tttt(1400,1125) [n="},
			       {"t#bar{t} [n=", "T1tttt(1150,500) [n=","T1tttt(1500,1) [n=","T1tttt(1500,1155) [n="}}, legCaption;
  TChain *chain[2][NHis];
  ReadChains(chain);
  
  // Finding yields in trees
  // vector<float> Ypt
  // vector<int>* lep_id;
  // vector<int>* njets;
  // vector<int>* nbm;
  // float weight;
  // float ht;
  // float met;
  // vector<double>* lep_pt;
  // for(int ene(0); ene < 2; ene++){
  //   for(int his(0); his < NHis; his++){
  //     chain[ene][his]->SetBranchAddress("weight", &weight);
  //     chain[ene][his]->SetBranchAddress("ht", &ht);
  //     chain[ene][his]->SetBranchAddress("met", &met);
  //     chain[ene][his]->SetBranchAddress("lep_pt", &lep_pt);
  //     chain[ene][his]->SetBranchAddress("lep_id", &lep_id);
  //     chain[ene][his]->SetBranchAddress("njets", &njets);
  //     chain[ene][his]->SetBranchAddress("nbm", &nbm);
  //     for(int entry(0); entry<chain[ene][his]->GetEntries(); entry++){
  // 	chain[ene][his]->GetEntry(entry);

  //     }
  //   }
  // }

  // Variables and cuts
  //TString allcuts = "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2";
  TString allcuts = "met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2";
  TString VarName[] = {"abs(lep_id)==13&&lep_pt>", "abs(lep_id)==11&&lep_pt>", "met>", "ht>",
		       "abs(lep_id)==13&&lep_pt>", "abs(lep_id)==11&&lep_pt>", "met>", "ht>"};
  TString Cuts[] = {"1", "1", "1", "1",
		    allcuts, allcuts, allcuts, allcuts};
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
  float maxHisto(-1), entries[2][NHis], means[2][NHis];
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
	Hname = "histo"; Hname += var; Hname += ene; Hname += his;
	hFile[var][ene][his] = new TH1F(Hname, "", nBins[var], Range[var][0], Range[var][1]);
	hFile[var][ene][his]->SetLineColor(colors[ene][his]);
	hFile[var][ene][his]->SetLineWidth(3);
	hFile[var][ene][his]->SetFillStyle(fillStyles[ene][his]);
	hFile[var][ene][his]->SetFillColor(colors[ene][his]);
	hFile[var][ene][his]->SetXTitle(xTitle);
	hFile[var][ene][his]->SetYTitle(yTitle);
	hFile[var][ene][his]->SetTitle(Title);
	for(int bin(1); bin <= nBins[var]; bin++){
	  totCut = "weight*(" + VarName[var];
	  totCut += hFile[var][ene][his]->GetBinLowEdge(bin); totCut += "&&";
	  totCut += Cuts[var]; totCut += ")";
	  chain[ene][his]->Project("hEntries", "ht", totCut);
	  hFile[var][ene][his]->SetBinContent(bin, hEntries.Integral());
	}
	cout<<ene<<", "<<his<<": "<<totCut<<endl;
	
	means[ene][his] = hFile[var][ene][his]->GetMean();
	double overflow = hFile[var][ene][his]->GetBinContent(nBins[var]+1);
	hFile[var][ene][his]->SetBinContent(nBins[var], hFile[var][ene][his]->GetBinContent(nBins[var])+overflow);
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
      for(int his(0); his < 2; his++){
	hFile[var][ene][his]->Scale(100./hFile[var][ene][his]->GetBinContent(1));
	if(maxHisto < hFile[var][ene][his]->GetMaximum()) maxHisto = hFile[var][ene][his]->GetMaximum();
      }
    } // Loop over energies

    leg.Clear(); //leg.SetY1NDC(legY-0.04);
    for(int his(0); his < 2; his++){
      for(int ene(0); ene < 2; ene++){
	legCaption = legNames[ene][his]; legCaption += energies[ene];
	legCaption.ReplaceAll("_"," "); legCaption.ReplaceAll(" [n="," @"); 
	leg.AddEntry(hFile[var][ene][his], legCaption);
	if(ene==0 && his==0) {
	  hFile[var][ene][his]->SetTitle("Shape comparison");
	  hFile[var][ene][his]->SetYTitle("Normalized efficiency (%)");
	  hFile[var][ene][his]->Draw();
	} else hFile[var][ene][his]->Draw("same");
      }
    } // Loop over energies
    leg.Draw();
    line.DrawLine(xcut,0,xcut,maxHisto*2);
    arrow.DrawArrow(xcut,maxHisto,xcut+0.04*(Range[var][1]-Range[var][0]),maxHisto);
    if(VarName[var]=="nel+nmu"){
      line.DrawLine(2,0,2,maxHisto);
      arrow.DrawArrow(2,maxHisto,2-0.04*(Range[var][1]-Range[var][0]),maxHisto);
    }
    Pname = "plots/nolog/shapes_"; Pname += var;  Pname += tags[var]; Pname += filetype;
    Pname.ReplaceAll("[","_"); Pname.ReplaceAll("]",""); Pname.ReplaceAll("+","-"); 
    can.SetLogy(0);    
    hFile[var][0][0]->SetMaximum(maxHisto*1.2);
    if(VarName[var]=="ntrupv") hFile[var][0][0]->SetMaximum(100);
    can.SaveAs(Pname);
    can.SetLogy(1);
    hFile[var][0][0]->SetMinimum(0.1);
    hFile[var][0][0]->SetMaximum(maxHisto*4);
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

void TableYields(TString cuts, TChain *chain[2][4], TString tag1, TString tag2, ofstream &file ){
  TString wcuts = "weight*("; wcuts += cuts; wcuts += ")";
  double yield[2][NHis];
  TString Hname = "histo";
  TH1F *histo = new TH1F(Hname, "",100, 0, 10000);
  for(int ene(0); ene < 2; ene++){
    for(int his(0); his < NHis; his++){
      chain[ene][his]->Project(Hname, "met", wcuts);
      yield[ene][his] = histo->Integral();
    }
  }
  histo->Delete();
  TString samples[] = {"$t\\bar{t}$    ", "T1ttt(1150,500)", "T1ttt(1500,0)  ", "T1ttt(1500,500)"};
  file << "\n\\begin{tabular}{l | rr | rr} \\hline\\hline\n"<<tag1<<" & \\multicolumn{2}{c|}{\\bf{\\underline{8 TeV}}} "
       <<"\n& \\multicolumn{2}{c}{\\bf{\\underline{13 TeV}}} \\\\"<<endl
       <<tag2<<" & \\multicolumn{1}{c}{$N$} & \\multicolumn{1}{c|}{$S/\\sqrt{B}$}"<<endl
       <<"& \\multicolumn{1}{c}{$N$} & \\multicolumn{1}{c}{$S/\\sqrt{B}$} \\\\ \\hline"<<endl;
  for(int his(0); his < NHis; his++){
    file<<samples[his];
    for(int ene(0); ene < 2; ene++){
      file<<" \t& "<< RoundNumber(yield[ene][his],0)<<" & ";
      if(his>0) file<< RoundNumber(yield[ene][his],2, sqrt(yield[ene][0]));
      else file<<" -";
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

  TableYields("1",chain,"","No cuts",file);
  TableYields("ht>500",chain,"","$H_T>500$",file);
  TableYields("met>450",chain,"","$E^{\\rm miss}_T>450$",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2",chain,
	      "","RA4 cuts",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[3]>=5&&nbm[1]>=2",chain,
	      "RA4 cuts","$N^{60\\text{ GeV}}_{\\rm jets}\\geq 5$",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[3]>=4&&nbm[1]>=2",chain,
	      "RA4 cuts","$N^{60\\text{ GeV}}_{\\rm jets}\\geq 4$",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[2]>=5&&nbm[1]>=2",chain,
	      "RA4 cuts","$N^{50\\text{ GeV}}_{\\rm jets}\\geq 5$",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[2]>=4&&nbm[1]>=2",chain,
	      "RA4 cuts","$N^{50\\text{ GeV}}_{\\rm jets}\\geq 4$",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>450&&ht>500&&njets[1]>=6&&nbm[1]>=2",chain,
	      "RA4 cuts", "$E^{\\rm miss}_T>450\\text{ GeV}$",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2&&dphi_wlep>1",chain,
	      "RA4 cuts", "$\\Delta\\phi>1$",file);
  TableYields("(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2&&mt>100",chain,
	      "RA4 cuts", "$m_T>100\\text{ GeV}$",file);
  file.close();
  cout<<"Written "<<name<<endl;
} 

