// energy_compare.C: Compares different variables at different energies

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

#define NVar 3  // Lepton isolation
//#define NVar 17  // 14-03-21 presentation

using namespace std;
using std::cout;
using std::endl;

void lep_isolation(TString filetype = ".eps"){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TString legNames[2][NHis] = {{"t#bar{t} [av=", "Signal @  8 TeV [cut at ","T1tttt(1400,25) [n=","T1tttt(1400,525) [n="},
			       {"t#bar{t} [av=", "Signal @ 13 TeV [cut at ","T1tttt(1500,1) [n=","T1tttt(1500,500) [n="}}, legCaption;
  TChain *chain[2][NHis];
  ReadChains(chain);

  // Variables and cuts
  TString VarName[] = {"lep_reliso", "lep_reliso", "lep_reliso"};
  TString Cuts[] = {"abs(lep_tru_momid)!=15&&abs(lep_tru_momid)<26", "abs(lep_tru_momid)==15", 
		    "abs(lep_tru_momid)>=26"};
  float Range[][2] = {{0,1}, {0,1}, {0,1}};
  int nBins[] = {100, 100, 100};
  TString tags[] = {"W", "tau", "other"};
  // Histograms and canvas
  TCanvas can;
  TH1F *hFile[NVar][2][NHis];
  int colors[2][NHis] = {{kRed-7, kRed+1, kGreen+1, kMagenta+1}, {kBlue-7, kBlue+1, kGreen+2, kMagenta+2}};
  //int fillStyles[2][NHis] = {{3345, 0, 0, 0}, {3354, 0, 0, 0}};
  TString xTitle = "", yTitle = "", Title = "", Pname, Hname, totCut;
  TString logtag = "_log"; logtag += filetype;
  float maxHisto(-1), fmax(1.2), fmax_log(4), entries[2][NHis], means[2][NHis];
  TLine line; line.SetLineColor(2); line.SetLineWidth(2); line.SetLineStyle(2);
  TArrow arrow; arrow.SetLineWidth(2); arrow.SetArrowSize(.02);
  const int doHis = 1;

  // Legend
  const double legX = 0.26, legY = 0.9;
  const double legW = 0.12, legH = 0.12;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  for(int var(0); var < NVar; var++){
    totCut = "weight*("; totCut += Cuts[var]; totCut += ")";

    int digits = 2;
    double xcut = 0.12;
    xTitle = "Lepton relative isolation";
    yTitle = "Entries/(";
    yTitle+= RoundNumber((Range[var][1]-Range[var][0]), digits, (double)nBins[var]);
    yTitle+=")";
    leg.Clear(); 
    
    for(int ene(0); ene < 2; ene++){
      Title = "Shape comparison"; 
      for(int his(doHis); his < doHis+1; his++){
	Hname = "histo"; Hname += var; Hname += ene; Hname += his;
	hFile[var][ene][his] = new TH1F(Hname, "", nBins[var], Range[var][0], Range[var][1]);
	hFile[var][ene][his]->SetLineColor(colors[ene][his]);
	hFile[var][ene][his]->SetLineWidth(3);
	//hFile[var][ene][his]->SetFillStyle(fillStyles[ene][his]);
	//hFile[var][ene][his]->SetFillColor(colors[ene][his]);
	hFile[var][ene][his]->SetXTitle(xTitle);
	hFile[var][ene][his]->SetYTitle(yTitle);
	hFile[var][ene][his]->SetTitle(Title);

	chain[ene][his]->Project(Hname, VarName[var], totCut);
	
	means[ene][his] = hFile[var][ene][his]->GetMean();
	double overflow = hFile[var][ene][his]->GetBinContent(nBins[var]+1);
	hFile[var][ene][his]->SetBinContent(nBins[var], hFile[var][ene][his]->GetBinContent(nBins[var])+overflow);
	entries[ene][his] = hFile[var][ene][his]->Integral();
      }
    } // Loop over energies

    maxHisto = -1;
    for(int his(doHis); his < doHis+1; his++){
      for(int ene(0); ene < 2; ene++){
	hFile[var][ene][his]->Scale(1000./entries[ene][his]);
	if(maxHisto < hFile[var][ene][his]->GetMaximum()) maxHisto = hFile[var][ene][his]->GetMaximum();

	// Legend
	int bincut = hFile[var][ene][his]->FindBin(xcut);
	legCaption = legNames[ene][his]; legCaption += RoundNumber(xcut,2); legCaption += " keeps ";
	legCaption += RoundNumber(hFile[var][ene][his]->Integral(1,bincut-1)*100, 1,
				  hFile[var][ene][his]->Integral(1,nBins[var]));
	legCaption += "% of leptons"; 
	legCaption += "]";
	leg.AddEntry(hFile[var][ene][his], legCaption);
	if(ene==0 && his==1) {
	  Title = "Shape comparison: leptons from "; Title += tags[var];
	  hFile[var][ene][his]->SetTitle(Title);
	  hFile[var][ene][his]->Draw();
	} else hFile[var][ene][his]->Draw("same");
      }
    } // Loop over energies
    leg.Draw();

    arrow.SetLineColor(colors[0][1]); 
    line.SetLineColor(colors[0][1]); 
    line.DrawLine(xcut,0,xcut,maxHisto*fmax);
    arrow.DrawArrow(xcut,maxHisto,xcut-0.04*(Range[var][1]-Range[var][0]),maxHisto);
    Pname = "plots/lep_iso/shapes_"; Pname += VarName[var];  Pname += tags[var]; Pname += filetype;
    Pname.ReplaceAll("[","_"); Pname.ReplaceAll("]",""); Pname.ReplaceAll("+","-"); 
    can.SetLogy(0);    
    hFile[var][0][1]->SetMaximum(maxHisto*fmax);
    can.SaveAs(Pname);
    can.SetLogy(1);
    hFile[var][0][1]->SetMinimum(0.1);
    hFile[var][0][1]->SetMaximum(maxHisto*fmax_log);
    line.DrawLine(xcut,0,xcut,maxHisto*fmax_log);
    Pname.ReplaceAll(filetype, logtag); 
    can.SaveAs(Pname);
  }
  for(int var(0); var < NVar; var++){
    for(int ene(0); ene < 2; ene++){
      for(int his(doHis); his < doHis+1; his++){
	if(hFile[var][ene][his]) hFile[var][ene][his]->Delete();
      }
    } // Loop over energies
  }
}

void energy_compare(TString filetype = ".eps"){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TString legNames[2][NHis] = {{"t#bar{t} [n=", "T1tttt(1150,525) [n=","T1tttt(1400,25) [n=","T1tttt(1400,525) [n="},
			       {"t#bar{t} [n=", "T1tttt(1150,500) [n=","T1tttt(1500,1) [n=","T1tttt(1500,500) [n="}}, legCaption;
  TChain *chain[2][NHis];
  ReadChains(chain);
  

  // Variables and cuts
  TString allcuts = "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2";
  TString VarName[] = {"dphi_wlep", "mt", "spher", "dr_bb",
		       "dphi_wlep", "mt", "spher", "dr_bb"};
  TString Cuts[] = {"1", "1", "1", "1",
		    allcuts, allcuts, allcuts, allcuts};
  float Range[][2] = {{0, 3.2}, {0, 800}, {0, 1}, {0, 6.5},
		      {0, 3.2}, {0, 800}, {0, 1}, {0, 6.5}};
  int nBins[] = {32, 40, 50, 65,
		 32, 40, 50, 65};
  TString tags[] = {"", "", "", "", "",
		    "allcuts", "allcuts", "allcuts", "allcuts"};
//   TString VarName[] = {"met", "ht", "ntrupv", "nel+nmu", "njets[3]", "njets[1]", "nbm[1]", "dphi_wlep", "mt",
// 		       "met", "ht", "nel+nmu", "njets[3]", "njets[1]", "nbm[1]", "dphi_wlep", "mt"};
//   TString Cuts[] = {"1", "1", "1/weight", "1", "1", "1", "1", "1", "1",
// 		    "(nvel+nvmu)==1&&(nel+nmu)==1&&ht>500&&njets[1]>=6&&nbm[1]>=2",
// 		    "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&njets[1]>=6&&nbm[1]>=2",
// 		    "ht>500&&met>250&&njets[1]>=6&&nbm[1]>=2",
// 		    "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500",
// 		    "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500",
// 		    "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500",
// 		    "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2",
// 		    "(nvel+nvmu)==1&&(nel+nmu)==1&&met>250&&ht>500&&njets[1]>=6&&nbm[1]>=2"};
//   float Range[][2] = {{0,1000}, {0,2200}, {0,60}, {0,6}, {0,15}, {0,15}, {0,7}, {0, 3.2}, {0, 800}, 
// 		      {0,1000}, {0,2200}, {0,6}, {0,15}, {0,15}, {0,7}, {0, 3.2}, {0, 800}};
//   int nBins[] = {50, 55, 60, 6, 15, 15, 7, 32, 40, 
// 		 50, 55, 6, 15, 15, 7, 32, 40};
//   TString tags[] = {"", "", "", "", "", "", "", "", "", 
// 		    "n-1", "n-1", "n-1", "n-1", "n-1", "n-1", "n-1", "n-1"};

  // Histograms and canvas
  TCanvas can;
  TH1F *hFile[NVar][2][NHis];
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
    //if(!VarName[var].Contains("dphi") && !VarName[var].Contains("mt")) continue;

    totCut = "weight*("; totCut += Cuts[var]; totCut += ")";

    int digits = 0;
    double xcut = -999.;
    xTitle = "";
    if(VarName[var]=="met") {xTitle = "E_{T,miss}"; xcut = 250;}
    if(VarName[var]=="ht") {xTitle = "H_{T}";  xcut = 500;}
    if(VarName[var]=="mt") xTitle = "m_{T}(#nu,lepton)"; 
    xTitle+=" (GeV)";
    if(VarName[var]=="ntrupv") xTitle = "Number of true PV"; 
    if(VarName[var]=="nel") xTitle = "Number of RA4 electrons"; 
    if(VarName[var]=="nmu") xTitle = "Number of RA4 muons"; 
    if(VarName[var]=="nel+nmu") {xTitle = "Number of RA4 leptons"; xcut=1;}
    if(VarName[var]=="njets[1]") {xTitle = "Number of 40+ GeV jets"; xcut=6;}
    if(VarName[var]=="nbm[1]") {xTitle = "Number of 40+ GeV b-tags (CSVM)"; xcut=2;}
    if(VarName[var]=="njets[3]") xTitle = "Number of 60+ GeV jets"; 
    if(VarName[var]=="nbm[3]") xTitle = "Number of 60+ GeV b-tags (CSVM)"; 
    if(VarName[var]=="dphi_wlep") {xTitle = "#Delta#phi(W,lepton)"; digits = 2;}
    if(VarName[var]=="spher") {xTitle = "Transverse sphericity"; digits = 2;}
    if(VarName[var]=="dr_bb") {xTitle = "#DeltaR(bb)"; digits = 2;}

    yTitle = "Entries ";
    if(VarName[var]=="met" || VarName[var]=="ht" || VarName[var]=="mt" || VarName[var]=="dphi_wlep" ||
       VarName[var]=="spher" || VarName[var]=="dr_bb"){
      yTitle+="/(";
      yTitle+= RoundNumber((Range[var][1]-Range[var][0]), digits, (double)nBins[var]);
      if(VarName[var]=="dphi_wlep") yTitle+=" rad)";
      else if(VarName[var]=="spher" || VarName[var]=="dr_bb") yTitle+=")";
      else yTitle+=" GeV)";
    }
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

	chain[ene][his]->Project(Hname, VarName[var], totCut);
	
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
      Pname = "plots/nolog/yields_"; Pname += VarName[var]; Pname += tags[var]; Pname += energies[ene]; Pname += filetype;
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

    maxHisto = -1;
    for(int ene(0); ene < 2; ene++){
      for(int his(0); his < 2; his++){
	hFile[var][ene][his]->Scale(1000./entries[ene][his]);
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
    Pname = "plots/nolog/shapes_"; Pname += VarName[var];  Pname += tags[var]; Pname += filetype;
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

