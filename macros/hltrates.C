// hltrates.C: Compares HLT rates of lepton pt, HT, and MET cuts

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
#include "TH2F.h"
#include "TMath.h"

#define NVar 2
#define NLeps 2

using namespace std;
using std::cout;
using std::endl;

TString RatesLine(int ht, int met, int pt, bool domu, TChain *chain[NSam], int noriginal[NSam]){
  TString filter = "!((onht>200||met>70)&&weight>1e6)";
  TString totCut = "(onht>"; totCut += ht;
  totCut += "&&met>"; totCut += met; totCut += "&&onelpt>"; totCut += pt;
  //totCut += ("&&"+filter);
  totCut += ")";
  TString Hname = "histo", cut;
  float yield[NSam];
  TH1F histo(Hname, "",100, 0, 10000);
  if(domu) totCut.ReplaceAll("onelpt","onmupt");
  for(int tag(0); tag < NSam; tag++){
    cut = totCut;
    if(tag>=2) cut = "1.4e-2/19600*weight*" + totCut;
    chain[tag]->Project(Hname, "met", cut);
    yield[tag] = histo.Integral();
    if(tag<2) yield[tag] /= ((double)noriginal[tag]/100.);
    cout<<"yield "<<yield[tag]<<" \t "<<cut<<endl;
  }
  yield[2] += (yield[3]+yield[4]);
  
  TString out = " "; out += ht; out += " & "; out += met;
  out += " & "; out += pt; 
  for(int tag(2); tag >= 0; tag--){
      out += " & ";out += RoundNumber(yield[tag],1);
  }
  out += " \\\\ \n";
  return out;
}

void RatesPrint(bool domu=true){
  TChain *chain[NSam];
  int noriginal[NSam];
  ReadChains(chain, noriginal);
  TString name = "txt/HLT_rates_el.tex";
  if(domu) name.ReplaceAll("el","mu");
  ofstream file(name);

  file << "\n\\begin{tabular}{ rrr | ccc}\\hline\\hline\n";
  file << " &&& \\multicolumn{1}{c}{\\bf{HLT rate}} ";
  file << " & \\multicolumn{1}{c}{\\bf{T1ttt(1500,100)}} ";
  file << " & \\multicolumn{1}{c}{\\bf{T1ttt(1200,800)}}\\\\ \n ";
  file << "$H_{T}$ & MET & $p_T^"; 
  if(domu) file<<"\\mu"; else file<<"e";
  file << "$ & (Hz) & $\\epsilon$ (\\%) & $\\epsilon$ (\\%)\\\\ \\hline \n";

  file << RatesLine(400, 65, 10, domu, chain, noriginal);
  file << RatesLine(660,  0, 10, domu, chain, noriginal);
  file << "\\hline";
  file << RatesLine(0,  100, 15, domu, chain, noriginal);
  file << RatesLine(250, 75, 15, domu, chain, noriginal);
  file << RatesLine(410, 55, 15, domu, chain, noriginal);
  file << RatesLine(500, 40, 15, domu, chain, noriginal);
  file << RatesLine(580,  0, 15, domu, chain, noriginal);
  file << "\\hline";
  file << RatesLine(400, 50, 20, domu, chain, noriginal);
  file << RatesLine(525,  0, 20, domu, chain, noriginal);
  file << "\\hline";
  file << RatesLine(390, 45, 25, domu, chain, noriginal);
  file << RatesLine(490,  0, 25, domu, chain, noriginal);

  file<< "\\hline\\hline\\end{tabular}"<<endl<<endl;
  file.close();
  cout<<"Written "<<name<<endl;
}

void hltrates(bool dofilter=true, TString filetype = ".eps"){
  styles style("2Dtitle"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
  TCanvas can;

  //Files
  TChain *chain[NSam];
  int noriginal[NSam];
  ReadChains(chain, noriginal);
  vector<int> indchain;
  indchain.push_back(3);
  indchain.push_back(2);
  indchain.push_back(4);
  indchain.push_back(0);
  indchain.push_back(1);

  // Histograms
  TString Hname, Title, totCut, Pname, dummy, Htag[]={"qcd","tt","wjets","sig1200","sig1500","tot"};
  TString xTitle = "Minimum online PF H_{T} (GeV)";
  TString yTitle = "Minimum offline PF E_{T,miss} (GeV)";
  TString zTitle = "HLT rate for L = 1.4 cm^{-2}s^{-1} (Hz)";
  TH2F *hRate[NLeps][6];
  TString filter = "!((onht>200||met>70)&&weight>1e6)";
  if(!dofilter) filter = "1";
  TString Cuts[] = {"onmupt>15.0", "onelpt>15.0", "onmupt>20.0"};
  int nBinsHt = 100, nBinsMet = 100;
  float minHt=0, maxHt=1000, minMet=0, maxMet=175;
  double levels[]={5,10,20};
  int lcolors[]={28, 1, 4};
  TString styles[]={"cont2 same", "cont3 same", "cont2 same"};
  TString legtag[]={"5 Hz", "10 Hz", "20 Hz"};
  double legX = 0.7, legY = 0.87;
  double legW = 0.12, legH = 0.2;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); //leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  for(int lep(0); lep < NLeps; lep++){
    Title = Cuts[lep]; Title.ReplaceAll("onmupt","p_{T}^{#mu}");
    Title.ReplaceAll("&&",", "); Title.ReplaceAll("ht","H_{T}^{off}");
    Title.ReplaceAll("onelpt","p_{T}^{e}"); Title.ReplaceAll(">"," > ");
    Title.ReplaceAll(".0"," GeV");
    totCut = "1.4e-2/19600*weight*(" + Cuts[lep] + "&&" + filter + ")";
    for(int tag(0); tag < 6; tag++){
      Hname = Htag[tag]; Hname += lep;
      hRate[lep][tag] = new TH2F(Hname,Title,nBinsHt, minHt, maxHt, nBinsMet, minMet, maxMet);
      hRate[lep][tag]->SetXTitle(xTitle);
      hRate[lep][tag]->SetYTitle(yTitle);
      hRate[lep][tag]->SetZTitle(zTitle);
      if(tag==3) {
	totCut.ReplaceAll("1.4e-2/19600*weight*","");
	hRate[lep][tag]->SetZTitle("T1ttt(1200,800) efficiency (%)");
      }
      if(tag==4) hRate[lep][tag]->SetZTitle("T1ttt(1500,100) efficiency (%)");
      if(tag<5) chain[indchain[tag]]->Project(Hname, "met:onht", totCut);
    }
    
    // Finding yields for different cuts
    for(int htbin(1); htbin <= nBinsHt; htbin++){
      for(int metbin(1); metbin <= nBinsMet; metbin++){
	float totrate(0);
	for(int tag(0); tag < 3; tag++)
	  totrate += hRate[lep][tag]->Integral(htbin, nBinsHt+1, metbin, nBinsMet+1);
	for(int tag(0); tag < 6; tag++)
	  if(tag<3||tag==5) hRate[lep][tag]->SetBinContent(htbin, metbin, totrate);
	  else hRate[lep][tag]->SetBinContent(htbin, metbin, hRate[lep][tag]->Integral(htbin, nBinsHt+1, metbin, nBinsMet+1));
      } // Loop over MET bins
    } // Loop over HT bins
    hRate[lep][5]->Draw("colz");
    leg.Clear();
    for(int tag(0); tag < 3; tag++){
      hRate[lep][tag]->SetContour(1,&levels[tag]); hRate[lep][tag]->SetLineColor(lcolors[tag]);
      hRate[lep][tag]->SetLineWidth(4);
      hRate[lep][tag]->Draw(styles[tag]);
      leg.AddEntry(hRate[lep][tag], legtag[tag]);
    }
    leg.Draw();
    dummy = Cuts[lep]; dummy.ReplaceAll(">",""); dummy.ReplaceAll("on",""); dummy.ReplaceAll(".0",""); 
    dummy.ReplaceAll("&&",""); 
    Pname = "plots/hlt/rate2d_"; Pname += dummy; 
    if(dofilter) Pname += "filter"; 
    Pname += filetype;
    can.SetLogz(1);
    can.SaveAs(Pname);

    // Plotting efficiencies
    hRate[lep][3]->Scale(100./(double)noriginal[indchain[3]]);
    can.SetLogz(0);
    //hRate[lep][3]->SetMaximum(70); hRate[lep][3]->SetMinimum(0);
    hRate[lep][3]->Draw("colz");
    for(int tag(0); tag < 3; tag++) hRate[lep][tag]->Draw(styles[tag]);
    Pname.ReplaceAll("rate2d","effi2d_1200");
    leg.Draw();
    can.SaveAs(Pname);
    hRate[lep][4]->Scale(100./(double)noriginal[indchain[4]]);
    //hRate[lep][4]->SetMaximum(70); hRate[lep][4]->SetMinimum(0);
    hRate[lep][4]->Draw("colz");
    for(int tag(0); tag < 3; tag++) hRate[lep][tag]->Draw(styles[tag]);
    Pname.ReplaceAll("effi2d_1200", "effi2d_1500");
    leg.Draw();
    can.SaveAs(Pname);

  } // Loop over lepton cuts

  for(int lep(0); lep < NLeps; lep++)
     for(int tag(0); tag < 6; tag++)
       if(hRate[lep][tag]) hRate[lep][tag]->Delete();
}

void hltrates1d(TString filetype = ".eps"){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TString legNames[] = {"T1tttt(1200,800)","T1tttt(1500,100)", "t#bar{t}", "QCD", "W + jets"}, legCaption;
  TChain *chain[NSam];
  int noriginal[NSam];
  ReadChains(chain, noriginal);
  vector<int> indchain;
  indchain.push_back(3);
  indchain.push_back(2);
  indchain.push_back(4);

  // Variables and cuts
  TString VarName[] = {"onht", "onmet", "onht", "onmet"};
  TString Cuts[] = {"onmupt>15", "onmupt>15", "onmupt>15&&ht>0", "onmupt>15&&ht>0"};
  TString tags[] = {"mu15", "mu15", "mu15ht0", "mu15ht0"};
  float Range[NVar][2];
  int nBins[NVar];
  for(int var(0); var < NVar; var++){
    if(VarName[var].Contains("onht")){
      Range[var][0] = 400;
      Range[var][1] = 1800;
      nBins[var] = 35;
    }
    if(VarName[var].Contains("onmet")){
      Range[var][0] = 30;
      Range[var][1] = 500;
      nBins[var] = 35;
    }
  }

  // Histograms and canvas
  TCanvas can;
  TH1F *hFile[NVar][NSam];
  TH1F hEntries("hEntries","",100,0,5000);
  int colors[] = {kRed-7, kGreen+1, 4, kBlue-7, kBlue+1, kGreen+2, kMagenta+2};
  TString xTitle = "", yTitle = "", Title = "", Pname, Hname, totCut;
  float maxHisto(-1), entries[NSam];
  double legX = 0.8, legY = 0.93;
  double legW = 0.12, legH = 0.2;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  TLine line; line.SetLineColor(2); line.SetLineWidth(2); line.SetLineStyle(2);
  TArrow arrow; arrow.SetLineWidth(2); arrow.SetArrowSize(.02);

  for(int var(0); var < NVar; var++){
    double xcut = -999.;
    xTitle = "";
    if(VarName[var].Contains("met")) {xTitle = "Minimum offline PF E_{T,miss}"; xcut = 50;}
    if(VarName[var].Contains("onmet")) {xTitle = "Minimum online PF E_{T,miss}"; xcut = 50;}
    if(VarName[var].Contains("ht")) {xTitle = "Minimum online PF H_{T}";  xcut = 350;}
    if(VarName[var].Contains("mupt")) {xTitle = "Cut on muon p_{T}"; xcut = 20;}
    if(VarName[var].Contains("elpt")) {xTitle = "Cut on electron p_{T}"; xcut = 20;}
    xTitle+=" (GeV)";
    yTitle = "HLT rate for L = 1.4 cm^{-2}s^{-1} (Hz)";
    leg.Clear(); leg.SetX1NDC(legX);leg.SetY2NDC(legY);

    for(unsigned int his(0); his < indchain.size(); his++){
      //Creating histogram
      Hname = "histo"; Hname += var; Hname += his;
      hFile[var][his] = new TH1F(Hname, "", nBins[var], Range[var][0], Range[var][1]);
      hFile[var][his]->SetLineColor(colors[his]);
      hFile[var][his]->SetLineWidth(3);
      hFile[var][his]->Sumw2();
      Title = Cuts[var]; Title.ReplaceAll("onmupt","p_{T}^{#mu}");
      Title.ReplaceAll("&&",", "); Title.ReplaceAll("ht","H_{T}^{off}");
      Title.ReplaceAll(">"," > ");
      hFile[var][his]->SetTitle(Title);
      hFile[var][his]->SetXTitle(xTitle);
      hFile[var][his]->SetYTitle(yTitle);

      // Finding yields for different cuts
      totCut = "1.4e-2/19600*weight*(" + VarName[var] + ">"; // Instant lumi 1.4e34 [1/cm2/s]
      totCut += hFile[var][his]->GetBinLowEdge(1); totCut += "&&";
      totCut += Cuts[var]; totCut += ")";
      chain[indchain[his]]->Project(Hname, VarName[var], totCut);
      for(int bin(1); bin <= nBins[var]; bin++){
	hFile[var][his]->SetBinContent(bin, hFile[var][his]->Integral(bin, nBins[var]+1));
	double w2(0);
	for(int ebin(bin); ebin <= nBins[var]+1; ebin++)
	  w2 += pow(hFile[var][his]->GetBinError(ebin),2);
	hFile[var][his]->SetBinError(bin, sqrt(w2));	
      }
	
      entries[his] = hFile[var][his]->Integral();

      if(maxHisto < hFile[var][his]->GetMaximum()) maxHisto = hFile[var][his]->GetMaximum();
    } // Loop over samples

    leg.Clear();
    for(unsigned int his(0); his < indchain.size(); his++){
      legCaption = legNames[indchain[his]]; 
      leg.AddEntry(hFile[var][his], legCaption);
      if(his==0) {
	hFile[var][his]->Draw();
      } else hFile[var][his]->Draw("same");
    }
    leg.Draw();
    Pname = "plots/hlt/rate_"; Pname += VarName[var]; Pname += tags[var]; Pname += filetype;
    Pname.ReplaceAll("[","_"); Pname.ReplaceAll("]","");  Pname.ReplaceAll("+","-"); 
    can.SetLogy(1);
    hFile[var][0]->SetMinimum(0.001);
    hFile[var][0]->SetMaximum(maxHisto*10);
    can.SaveAs(Pname);

  } // Loop over all variables


  for(int var(0); var < NVar; var++){
    for(unsigned int his(0); his < indchain.size(); his++){
      if(hFile[var][his]) hFile[var][his]->Delete();
    }
  }
}


