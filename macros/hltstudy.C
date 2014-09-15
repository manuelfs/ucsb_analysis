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

#define NSam 6
#define NCuts 3

using namespace std;
using std::cout;
using std::endl;

void ReadChains(TChain *chain[], int entries[], TString folder);

void hltstudy(TString folder="root/hlt/el15noiso/", TString addtitle = ""){
  styles style("2Dtitle"); style.setDefaultStyle(); gStyle->SetPaintTextFormat("4.1f");
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
  TH2F *hRate[NCuts][NSam+1], *hContour[NCuts][4];

  TString eliso5x = "Max$(els_pt*(els_trackiso<0.25&&(els_eta<1.479&&els_ecaliso<1||els_eta>=1.479&&els_ecaliso<0.75)&&els_hcaliso<0.55))>15";
  TString muiso5x = "Max$(mus_pt*(mus_reliso<1))>15";
  TString Cuts[] = {"1", "iso5x", "iso5x&&Max$(bjets_csv*(bjets_pt>40))>0.7"};
  TString Tags[] = {"noiso", "iso5x", "iso5xbtag"};
  TString TitleCuts[] = {"", ", RelIso < iso5x", ", RelIso < iso5x, CSV > 0.7"};
  TString tagfolder = folder;
  if(tagfolder[tagfolder.Sizeof()-2] == '/') tagfolder.Remove(tagfolder.Sizeof()-2);
  tagfolder.Remove(0,tagfolder.Last('/')+1);

  int nBinsHt = 14, nBinsMet = 14;
  float minHt=200, maxHt=900, minMet=0, maxMet=140;

  double legX = 0.72, legY = 0.87;
  double legW = 0.12, legH = 0.2;
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.056); leg.SetFillColor(0); //leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);
  vector<double> levels, htlevels;
  levels.push_back(1); levels.push_back(2); 
  levels.push_back(5); levels.push_back(10); 
  // htlevels.push_back(400); htlevels.push_back(500); 
  // htlevels.push_back(600); htlevels.push_back(700); 
  //int lcolors[]={2, 28, 1, 4};
  TString styles[]={"cont3 same", "cont3 same", "cont3 same", "cont3 same"}, Title;
  vector<TString>legtag;
  for(unsigned tag(0); tag < levels.size(); tag++){
    legtag.push_back(""); 
    if(levels[tag]<1) legtag[tag] += RoundNumber(levels[tag],2);
    else legtag[tag] += levels[tag];
    legtag[tag] += " Hz";
  }
  
  for(int icut(0); icut < NCuts; icut++){
    totCut = "1.4e-2/19600*weight*(" + Cuts[icut] + ")";
    Title = "p_{T}^{#mu} > 15 GeV"+TitleCuts[icut];
    if(isEl) {
      Title.ReplaceAll("#mu","e");
      Title.ReplaceAll("iso5x", "1.7");
      totCut.ReplaceAll("iso5x", eliso5x);
    } else {
      totCut.ReplaceAll("iso5x", muiso5x);
      Title.ReplaceAll("iso5x", "1");
    }
    if(addtitle!="") Title += (", "+addtitle);
    cout<<endl<<"Doing cuts "<<totCut<<endl;
    for(int tag(0); tag < 4; tag++){
      Hname = Htag[tag]+"contour"; Hname += icut;
      hContour[icut][tag] = new TH2F(Hname,Title,nBinsHt, minHt, maxHt, nBinsMet, minMet, maxMet);
      hContour[icut][tag]->SetMarkerStyle(20);
      hContour[icut][tag]->SetMarkerSize(2.2);
    }      
    for(int sam(0); sam < NSam+1; sam++){
      Hname = Htag[sam]; Hname += icut;
      hRate[icut][sam] = new TH2F(Hname,Title+"          L = 1.4 #times 10^{34} cm^{-2}s^{-1}",
				 nBinsHt, minHt, maxHt, nBinsMet, minMet, maxMet);
      hRate[icut][sam]->SetXTitle(xTitle);
      hRate[icut][sam]->SetYTitle(yTitle);
      hRate[icut][sam]->SetZTitle(Htag[sam]+" "+zTitle);
      if(Htag[sam].Contains("sig")) hRate[icut][sam]->SetTitle(Title);
      if(sam==NSam-3) {
	totCut.ReplaceAll("1.4e-2/19600*weight*(","wl1ht200*(Max$(genmus_pt[0])>0&&");
	if(isEl) totCut.ReplaceAll("genmus", "genels");
	hRate[icut][sam]->SetZTitle("T1tttt(1025,625) efficiency (%)");	
      }
      if(sam==NSam-2) hRate[icut][sam]->SetZTitle("T1tttt(1200,800) efficiency (%)");	
      if(sam==NSam-1) hRate[icut][sam]->SetZTitle("T1tttt(1500,100) efficiency (%)");
      if(sam<NSam) chain[indchain[sam]]->Project(Hname, "abs(onmet):onht", totCut);
      // if(sam>=NSam-3 && sam<=NSam-1){
      // 	hRate[icut][sam]->SetMarkerStyle(20);
      // 	hRate[icut][sam]->SetMarkerSize(0.5);
      // 	hRate[icut][sam]->Draw("scat=2");
      // 	Pname = "plots/hltstudy/scatter_"+tagfolder + "_"+Tags[icut]+ ".eps";
      // 	can.SaveAs(Pname);
      // }
      if(Htag[sam]=="Total") hRate[icut][sam]->SetZTitle("HLT rate [QCD + tt + W#rightarrowl#nu] (Hz)");
    }

    // Finding yields for different cuts
    unsigned int iht(0); int itag(3);
    for(int htbin(1); htbin <= nBinsHt; htbin++){
      for(int metbin(1); metbin <= nBinsMet; metbin++){
	float bkgrate(0), rate;
	for(int sam(0); sam < NSam; sam++){
	  rate = hRate[icut][sam]->Integral(htbin, nBinsHt+1, metbin, nBinsMet+1);
	  if(sam < NSam-3) bkgrate += rate;
	  else rate *= (100./(double)noriginal[indchain[sam]]);
	  hRate[icut][sam]->SetBinContent(htbin, metbin, rate);
	}
	hRate[icut][NSam]->SetBinContent(htbin, metbin, bkgrate);
	hContour[icut][3]->SetBinContent(htbin, metbin, bkgrate);
	// Filling contour histograms and printing cuts for desired rate levels
	for(int tag(0); tag < 3; tag++){
	  hContour[icut][tag]->SetBinContent(htbin, metbin, hRate[icut][NSam-3+tag]->GetBinContent(htbin, metbin));
	  if(iht < htlevels.size()){
	    if(hContour[icut][tag]->GetXaxis()->GetBinLowEdge(htbin) >= htlevels[iht]){
	      if(itag==tag && bkgrate<=levels[itag] || metbin==nBinsMet) {
		if(itag==3) 
		  cout<<"-- HT > "<<RoundNumber(hContour[icut][tag]->GetXaxis()->GetBinLowEdge(htbin),1)<<endl;
		cout<<"MET > "<<RoundNumber(hContour[icut][tag]->GetYaxis()->GetBinLowEdge(metbin),1)
		    <<": \t"<<RoundNumber(bkgrate,2)<<" Hz, \teff1200 "
		    <<RoundNumber(hRate[icut][NSam-2]->GetBinContent(htbin, metbin),1)<<" %, \teff1500 "
		    <<RoundNumber(hRate[icut][NSam-1]->GetBinContent(htbin, metbin),1)<<endl;
		itag--;
		if(itag==-1) {iht++; itag=3; cout<<endl;}
	      }
	    }
	  }
	} // Loop over contour lines
      } // Loop over MET bins
    } // Loop over HT bins
    hRate[icut][NSam]->Draw("colz");
    leg.Clear();
    for(int tag(0); tag < 4; tag++){
      //hContour[icut][tag]->SetContour(1,&levels[tag]); hContour[icut][tag]->SetLineColor(lcolors[tag]);
      hContour[icut][tag]->SetLineWidth(4);
      hContour[icut][tag]->Draw(styles[tag]);
      leg.AddEntry(hContour[icut][tag], legtag[tag]);
    }
    // Plotting rates and efficiencies
    Pnamebase = "plots/hltstudy/rate2d_"+tagfolder + "_"+Tags[icut]+ ".eps";
    for(int sam(0); sam < NSam+1; sam++){
      if(Htag[sam].Contains("sig1200") ||Htag[sam].Contains("sig1500")) continue;
      Pname = Pnamebase; 
      if(Htag[sam].Contains("sig")) {
	Pname.ReplaceAll("rate2d", Htag[sam]+"effi2d");
	can.SetLogz(0);
      } else {
	hRate[icut][sam]->SetMinimum(0.01);
	hRate[icut][sam]->SetMaximum(2*hRate[icut][NSam]->GetMaximum());
	Pname.ReplaceAll("rate2d", Htag[sam]+"rate2d");
	can.SetLogz(1);
      }
      hRate[icut][sam]->Draw("colz");
      if(sam>=NSam-3) {
	for(int tag(0); tag < 1; tag++) hContour[icut][sam-NSam+3]->Draw("text same");
	//leg.Draw();
      }
      can.SaveAs(Pname);
    }
  } // Loop over cuts

  for(int icut(0); icut < NCuts; icut++){
     for(int tag(0); tag < NSam+1; tag++)
       if(hRate[icut][tag]) hRate[icut][tag]->Delete();
     for(int tag(0); tag < 4; tag++)
       if(hContour[icut][tag]) hContour[icut][tag]->Delete();
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



void ReadChains(TChain *chain[], int entries[], TString folder){
  TString FileNames[] = 
    {folder+"/*T1tttt*1025_*", 
     folder+"/*T1tttt*1025_*", 
     folder+"/*T1tttt*1025_*", 
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
