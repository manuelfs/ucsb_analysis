// hltstudy.C: Compares HLT rates of lepton pt, HT, and MET cuts

#define INT_ROOT
#include "inc/styles.hpp"
#include "src/styles.cpp"
#include "inc/ucsb_utils.hpp"
#include "src/ucsb_utils.cpp"


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
#define NSam 6
#define NLeps 1

using namespace std;
using std::cout;
using std::endl;

void ReadChains(TChain *chain[], int entries[], TString folder);

void hltstudy(TString folder="root/hlt/ht200/mu15/", TString filetype = ".eps"){
  styles style("2Dtitle"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
  TCanvas can;
  bool isEl = folder.Contains("el")?true:false;

  //Files
  TChain *chain[NSam];
  int noriginal[NSam];
  ReadChains(chain, noriginal, folder);
  vector<int> indchain;
  indchain.push_back(3);
  indchain.push_back(4);
  indchain.push_back(5);
  indchain.push_back(0);
  indchain.push_back(1);
  indchain.push_back(2);

  // Histograms
  TString Hname, totCut, Pnamebase, Pname, Htag[]={"QCD","tt","Wjets","sig1025","sig1200", "sig1500","Total"};
  TString xTitle = "Minimum HLT PF H_{T} (GeV)";
  TString yTitle = "Minimum HLT PF E_{T,miss} (GeV)";
  TString zTitle = "HLT rate (Hz)";
  TH2F *hRate[NLeps][NSam+1], *hContour[NLeps][4];
  //TString Cuts[] = {"onmupt>15.0", "onmupt>20.0", "onmupt>20.0"};
  TString Cuts[] = {"1", "1", "1"};
  //TString Cuts[] = {"((abs(jets_phi[0]-jets_phi[1])*(abs(jets_phi[0]-jets_phi[1])<3.1416)+(2*3.1416-abs(jets_phi[0]-jets_phi[1]))*(abs(jets_phi[0]-jets_phi[1])>=3.1416))>2.8)", "1", "1"};
  int nBinsHt = 100, nBinsMet = 90;
  float minHt=200, maxHt=1000, minMet=0, maxMet=180;

  double legX = 0.72, legY = 0.87;
  double legW = 0.12, legH = 0.2;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); //leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  vector<double> levels, htlevels;
  levels.push_back(1); levels.push_back(2); 
  levels.push_back(5); levels.push_back(10); 
  htlevels.push_back(400); htlevels.push_back(500); 
  htlevels.push_back(600); htlevels.push_back(700); 
  int lcolors[]={2, 28, 1, 4};
  TString styles[]={"cont3 same", "cont3 same", "cont3 same", "cont3 same"};
  vector<TString>legtag;
  for(unsigned tag(0); tag < levels.size(); tag++){
    legtag.push_back(""); 
    if(levels[tag]<1) legtag[tag] += RoundNumber(levels[tag],2);
    else legtag[tag] += levels[tag];
    legtag[tag] += " Hz";
  }
  
  TString Title = "p_{T}^{#mu} > 15 GeV";
  if(isEl) Title.ReplaceAll("#mu","e");
  for(int lep(0); lep < NLeps; lep++){
    totCut = "1.4e-2/19600*weight*(" + Cuts[lep] + ")";
    for(int tag(0); tag < 4; tag++){
      Hname = Htag[tag]+"contour"; Hname += lep;
      hContour[lep][tag] = new TH2F(Hname,Title,nBinsHt, minHt, maxHt, nBinsMet, minMet, maxMet);
    }      
    for(int sam(0); sam < NSam+1; sam++){
      Hname = Htag[sam]; Hname += lep;
      hRate[lep][sam] = new TH2F(Hname,Title+", L = 1.4 #times 10^{34} cm^{-2}s^{-1}",
				 nBinsHt, minHt, maxHt, nBinsMet, minMet, maxMet);
      hRate[lep][sam]->SetXTitle(xTitle);
      hRate[lep][sam]->SetYTitle(yTitle);
      hRate[lep][sam]->SetZTitle(Htag[sam]+" "+zTitle);
      if(Htag[sam].Contains("sig")) hRate[lep][sam]->SetTitle(Title);
      if(sam==NSam-3) {
	totCut.ReplaceAll("1.4e-2/19600*weight*(","wl1ht200*(Max$(genmus_pt[0])>0&&");
	if(isEl) totCut.ReplaceAll("genmus", "genels");
	hRate[lep][sam]->SetZTitle("T1tttt(1025,625) efficiency (%)");	
      }
      if(sam==NSam-2) hRate[lep][sam]->SetZTitle("T1tttt(1200,800) efficiency (%)");	
      if(sam==NSam-1) hRate[lep][sam]->SetZTitle("T1tttt(1500,100) efficiency (%)");
      if(sam<NSam) chain[indchain[sam]]->Project(Hname, "abs(onmet):onht", totCut);
    }

    // Finding yields for different cuts
    unsigned int iht(0); int itag(3);
    for(int htbin(1); htbin <= nBinsHt; htbin++){
      for(int metbin(1); metbin <= nBinsMet; metbin++){
	float bkgrate(0), rate;
	for(int sam(0); sam < NSam; sam++){
	  rate = hRate[lep][sam]->Integral(htbin, nBinsHt+1, metbin, nBinsMet+1);
	  if(sam < NSam-3) bkgrate += rate;
	  else rate *= (100./(double)noriginal[indchain[sam]]);
	  hRate[lep][sam]->SetBinContent(htbin, metbin, rate);
	}
	hRate[lep][NSam]->SetBinContent(htbin, metbin, bkgrate);
	// Filling contour histograms and printing cuts for desired rate levels
	for(int tag(0); tag < 4; tag++){
	  if(iht < htlevels.size()){
	    if(hContour[lep][tag]->GetXaxis()->GetBinLowEdge(htbin) >= htlevels[iht]){
	      if(itag==tag && bkgrate<=levels[itag] || metbin==nBinsMet) {
		if(itag==3) 
		  cout<<"-- HT > "<<RoundNumber(hContour[lep][tag]->GetXaxis()->GetBinLowEdge(htbin),1)<<endl;
		cout<<"MET > "<<RoundNumber(hContour[lep][tag]->GetYaxis()->GetBinLowEdge(metbin),1)
		    <<": \t"<<RoundNumber(bkgrate,2)<<" Hz, \teff1200 "
		    <<RoundNumber(hRate[lep][NSam-2]->GetBinContent(htbin, metbin),1)<<" %, \teff1500 "
		    <<RoundNumber(hRate[lep][NSam-1]->GetBinContent(htbin, metbin),1)<<endl;
		itag--;
		if(itag==-1) {iht++; itag=3; cout<<endl;}
	      }
	    }
	  }
	  hContour[lep][tag]->SetBinContent(htbin, metbin, bkgrate);
	} // Loop over contour lines
      } // Loop over MET bins
    } // Loop over HT bins
    hRate[lep][NSam]->Draw("colz");
    leg.Clear();
    for(int tag(0); tag < 4; tag++){
      hContour[lep][tag]->SetContour(1,&levels[tag]); hContour[lep][tag]->SetLineColor(lcolors[tag]);
      hContour[lep][tag]->SetLineWidth(4);
      hContour[lep][tag]->Draw(styles[tag]);
      leg.AddEntry(hContour[lep][tag], legtag[tag]);
    }
    // Plotting rates and efficiencies
    Pnamebase = "plots/hltstudy/rate2d_"; Pnamebase += (isEl?"el":"mu"); 
    Pnamebase += filetype;
    for(int sam(0); sam < NSam+1; sam++){
      Pname = Pnamebase; 
      if(Htag[sam].Contains("sig")) {
	Pname.ReplaceAll("rate2d", Htag[sam]+"effi2d");
	can.SetLogz(0);
      } else {
	hRate[lep][sam]->SetMinimum(0.01);
	hRate[lep][sam]->SetMaximum(2*hRate[lep][NSam]->GetMaximum());
	Pname.ReplaceAll("rate2d", Htag[sam]+"rate2d");
	can.SetLogz(1);
      }
      hRate[lep][sam]->Draw("colz");
      if(sam>=NSam-3) for(int tag(0); tag < 4; tag++) hContour[lep][tag]->Draw(styles[tag]);
      leg.Draw();
      can.SaveAs(Pname);
    }
  } // Loop over lepton cuts

  for(int lep(0); lep < NLeps; lep++){
     for(int tag(0); tag < NSam+1; tag++)
       if(hRate[lep][tag]) hRate[lep][tag]->Delete();
     for(int tag(0); tag < 4; tag++)
       if(hContour[lep][tag]) hContour[lep][tag]->Delete();
  }
}

TString RatesLine(int ht, int met, int pt, bool domu, TChain *chain[NSam], int noriginal[NSam]);

void RatesPrint(TString folder="root/hlt/sep4/el15"){
  bool domu(folder.Contains("mu")?true:false);
  bool do711(folder.Contains("711")?true:false);
  TChain *chain[NSam];
  int noriginal[NSam];
  ReadChains(chain, noriginal, folder);
  TString name = "txt/hlt_rates_el710.tex";
  if(domu) name.ReplaceAll("el","mu");
  if(do711) name.ReplaceAll("710","711");
  ofstream file(name);

  file << "\n\\begin{tabular}{ rrr | ccc}\\hline\\hline\n";
  file << " \\multicolumn{3}{c|}{\\bf{";
  if(do711) file << "7.1.1 path";
  else      file << "7.1.0 path";
  file<< "}} & \\multicolumn{1}{c}{\\bf{HLT rate}} ";
  file << " & \\multicolumn{1}{c}{\\bf{T1tttt(1025,625)}} ";
  file << " & \\multicolumn{1}{c}{\\bf{T1tttt(825,525)}}\\\\ \n ";
  file << "$H_{T}$ & MET & $p_T^"; 
  if(domu) file<<"\\mu"; else file<<"e";
  file << "$ & (Hz) & $\\epsilon$ (\\%) & $\\epsilon$ (\\%)\\\\ \\hline \n";

  if(!domu && !do711){
    file << RatesLine(400, 103, 15, domu, chain, noriginal);
    file << RatesLine(500,  87, 15, domu, chain, noriginal);
    file << RatesLine(600,  70, 15, domu, chain, noriginal);
    file << RatesLine(700,  48, 15, domu, chain, noriginal);
  }
  if(!domu && do711){
    file << RatesLine(400, 107, 15, domu, chain, noriginal);
    file << RatesLine(500,  91, 15, domu, chain, noriginal);
    file << RatesLine(600,  75, 15, domu, chain, noriginal);
    file << RatesLine(700,  57, 15, domu, chain, noriginal);
  }
  if(domu && !do711){
    file << RatesLine(400, 112, 15, domu, chain, noriginal);
    file << RatesLine(500, 100, 15, domu, chain, noriginal);
    file << RatesLine(600,  85, 15, domu, chain, noriginal);
    file << RatesLine(700,  65, 15, domu, chain, noriginal);
  }
  if(domu && do711){
    file << RatesLine(400, 113, 15, domu, chain, noriginal);
    file << RatesLine(500, 100, 15, domu, chain, noriginal);
    file << RatesLine(600,  85, 15, domu, chain, noriginal);
    file << RatesLine(700,  65, 15, domu, chain, noriginal);
  }
  file<< "\\hline\\hline\\end{tabular}"<<endl<<endl;
  file.close();
  cout<<"Written "<<name<<endl;
}

void ratesAll(){
  RatesPrint("root/hlt/conf710/el15/");
  RatesPrint("root/hlt/conf711/el15/");
  RatesPrint("root/hlt/conf710/mu15/");
  RatesPrint("root/hlt/conf711/mu15/");
}

TString RatesLine(int ht, int met, int pt, bool domu, TChain *chain[NSam], int noriginal[NSam]){
  TString totCut = "(onht>"; totCut += ht;
  totCut += "&&onmet>"; totCut += met; //totCut += "&&onelpt>"; totCut += pt;
  totCut += ")";
  TString Hname = "histo", cut;
  float yield[NSam];
  TH1F histo(Hname, "",100, 0, 10000);
  if(domu) totCut.ReplaceAll("onelpt","onmupt");
  for(int tag(0); tag < NSam; tag++){
    cut = totCut;
    if(tag>=2) cut = "1.4e-2/19600*weight*" + totCut;
    chain[tag]->Project(Hname, "onmet", cut);
    yield[tag] = histo.Integral();
    if(tag<2) yield[tag] /= ((double)noriginal[tag]/100.);
    //cout<<"yield "<<yield[tag]<<" \t "<<cut<<endl;
  }
  //yield[2] += (yield[3]+yield[4]);
  yield[2] += yield[3]; // Adding tt to QCD
  
  TString out = " "; out += ht; out += " & "; out += met;
  out += " & "; out += pt; 
  for(int tag(2); tag >= 0; tag--){
    int digits(tag==2?2:1);
    out += " & ";out += RoundNumber(yield[tag],digits);
  }
  out += " \\\\ \n";
  return out;
}


// Not fully adapted to ntuples from HLT babies
void hltstudy1d(TString folder="root/hlt/conf710/el15", bool owen=true, TString filetype = ".eps"){
  styles style("Standard"); style.setDefaultStyle();
  gStyle->SetHatchesLineWidth(2);
 
  //Files
  TString legNames[] = {"T1tttt(825,525)","T1tttt(1025,625)", "t#bar{t}", "QCD", "W + jets"}, legCaption;
  TChain *chain[NSam];
  int noriginal[NSam];
  ReadChains(chain, noriginal, folder);
  vector<int> indchain;
  indchain.push_back(3);
  indchain.push_back(2);
  if(!owen) indchain.push_back(4);

  // Variables and cuts
  TString VarName[] = {"onht", "onmet", "onht", "onmet"};
  //TString Cuts[] = {"onmupt>15", "onmupt>15", "onmupt>15&&ht>0", "onmupt>15&&ht>0"};
  TString Cuts[] = {"1", "1", "ht>0", "ht>0"};
  //TString Cuts[] = {"1", "1", "onmupt>15&&ht>0", "onmupt>15&&ht>0"};
  TString tags[] = {"", "", "ht0", "ht0"};
  float Range[NVar][2];
  int nBins[NVar];
  for(int var(0); var < NVar; var++){
    if(VarName[var].Contains("ht")){
      Range[var][0] = 400;
      Range[var][1] = 1800;
      nBins[var] = 35;
    }
    if(VarName[var].Contains("met")){
      Range[var][0] = 30;
      Range[var][1] = 500;
      nBins[var] = 35;
      if(owen){
      Range[var][0] = 80;
      Range[var][1] = 330;
      }
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
      if(Title=="1") Title = "";
      hFile[var][his]->SetTitle(Title);
      hFile[var][his]->SetXTitle(xTitle);
      hFile[var][his]->SetYTitle(yTitle);

      // Finding yields for different cuts
      totCut = "1.4e-2/19600*weight*(" + VarName[var] + ">"; // Instant lumi 1.4e34 [1/cm2/s]
      totCut += hFile[var][his]->GetBinLowEdge(1); totCut += "&&";
      totCut += (Cuts[var]  + ")");
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
    Pname = "plots/hltstudy/rate_"; Pname += VarName[var]; Pname += tags[var]; 
    Pname += filetype;
    Pname.ReplaceAll("[","_"); Pname.ReplaceAll("]","");  Pname.ReplaceAll("+","-"); 
    can.SetLogy(1);
    hFile[var][0]->SetMinimum(0.001);
    hFile[var][0]->SetMaximum(maxHisto*10);
    if(owen){
      hFile[var][0]->SetMinimum(0.2);
      hFile[var][0]->SetMaximum(20000);
    }
    can.SaveAs(Pname);

  } // Loop over all variables


  for(int var(0); var < NVar; var++){
    for(unsigned int his(0); his < indchain.size(); his++){
      if(hFile[var][his]) hFile[var][his]->Delete();
    }
  }
}


void ReadChains(TChain *chain[], int entries[], TString folder){
  TString FileNames[] = 
    {folder+"/*T1tttt*1025_*", 
     folder+"/*T1tttt*1200_*", 
     folder+"/*T1tttt*1500_*", 
     folder+"/*QCD*",
     folder+"/*TT*",
     folder+"/*WJets*"};

  bool isEl = folder.Contains("el")?true:false;
  for(int sam(0); sam < NSam; sam++){
    chain[sam] = new TChain("tree");
    chain[sam]->Add(FileNames[sam]);
    if(FileNames[sam].Contains("T1tttt")) {
      int noriginal, nori_genmu0, nori_genel0;
      TChain tglobal("treeglobal");
      tglobal.Add(FileNames[sam]);
      tglobal.SetBranchAddress("noriginal",&noriginal);
      tglobal.SetBranchAddress("nori_genel0",&nori_genel0);
      tglobal.SetBranchAddress("nori_genmu0",&nori_genmu0);
      tglobal.GetEntry(0);
      if(isEl) entries[sam] = nori_genel0;
      else entries[sam] = nori_genmu0;
    }
  }// Loop over samples
}
