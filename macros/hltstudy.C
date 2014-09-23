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
  TH2F *hRate[2][NCuts][NSam+1], *hContour[NCuts][4];

  TString eliso5x = "Max$(els_pt*(els_trackiso<0.25&&(els_eta<1.479&&els_ecaliso<1||els_eta>=1.479&&els_ecaliso<0.75)&&els_hcaliso<0.55))>15";
  TString muiso5x = "Max$(mus_pt*(mus_reliso<1))>15";
  TString Cuts[] = {"1", "iso5x", "iso5x&&Max$(bjets_csv*(bjets_pt>40))>0.7"};
  TString Tags[] = {"noiso", "iso5x", "iso5xbtag"};
  TString TitleCuts[] = {"", ", RelIso < iso5x", ", RelIso < iso5x, CSV > 0.7"};
  // TString tagfolder = folder;
  // if(tagfolder[tagfolder.Sizeof()-2] == '/') tagfolder.Remove(tagfolder.Sizeof()-2);
  // tagfolder.Remove(0,tagfolder.Last('/')+1);

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
      if(tag<3) hContour[icut][tag]->SetMarkerSize(2.2);
      else hContour[icut][tag]->SetMarkerSize(2.5);
    }      
    for(int sam(0); sam < NSam+1; sam++){
      for(int his(1); his >= 0; his--) {
	Hname = Htag[sam]; Hname += icut; Hname += his;
	hRate[his][icut][sam] = new TH2F(Hname,Title+"          L = 1.4 #times 10^{34} cm^{-2}s^{-1}",
					 nBinsHt, minHt, maxHt, nBinsMet, minMet, maxMet);
      }
      hRate[0][icut][sam]->SetXTitle(xTitle);
      hRate[0][icut][sam]->SetYTitle(yTitle);
      hRate[0][icut][sam]->SetZTitle(Htag[sam]+" "+zTitle);
      if(Htag[sam].Contains("sig")) hRate[0][icut][sam]->SetTitle(Title);
      if(sam==NSam-3) {
	totCut.ReplaceAll("1.4e-2/19600*weight*(","wl1ht200*(Max$(genmus_pt[0])>0&&");
	if(isEl) totCut.ReplaceAll("genmus", "genels");
	hRate[0][icut][sam]->SetZTitle("T1tttt(1025,625) efficiency (%)");	
      }
      if(sam==NSam-2) hRate[0][icut][sam]->SetZTitle("T1tttt(1200,800) efficiency (%)");	
      if(sam==NSam-1) hRate[0][icut][sam]->SetZTitle("T1tttt(1500,100) efficiency (%)");
      if(sam<NSam) chain[indchain[sam]]->Project(Hname, "abs(onmet):onht", totCut);
      // if(sam>=NSam-3 && sam<=NSam-1){
      // 	hRate[0][icut][sam]->SetMarkerStyle(20);
      // 	hRate[0][icut][sam]->SetMarkerSize(0.5);
      // 	hRate[0][icut][sam]->Draw("scat=2");
      // 	Pname = "plots/hltstudy/scatter_"+tagfolder + "_"+Tags[icut]+ ".eps";
      // 	can.SaveAs(Pname);
      // }
      if(Htag[sam]=="Total") hRate[0][icut][sam]->SetZTitle("HLT rate [QCD + tt + W#rightarrowl#nu] (Hz)");
      hRate[1][icut][sam]->SetMarkerStyle(20);
      if(sam>=NSam-3 && sam<=NSam-1) hRate[1][icut][sam]->SetMarkerSize(2.2);
      else hRate[1][icut][sam]->SetMarkerSize(2.5);
    }

    // Finding yields for different cuts
    unsigned int iht(0); int itag(3);
    for(int htbin(1); htbin <= nBinsHt; htbin++){
      for(int metbin(1); metbin <= nBinsMet; metbin++){
    	float bkgrate(0), rate;
    	for(int sam(0); sam < NSam; sam++){
    	  rate = hRate[0][icut][sam]->Integral(htbin, nBinsHt+1, metbin, nBinsMet+1);
    	  if(sam < NSam-3) bkgrate += rate;
    	  else rate *= (100./(double)noriginal[indchain[sam]]);
    	  for(int his(0); his < 2; his++) 
    	    hRate[his][icut][sam]->SetBinContent(htbin, metbin, rate);
    	}
    	for(int his(0); his < 2; his++) 
    	  hRate[his][icut][NSam]->SetBinContent(htbin, metbin, bkgrate);
    	hContour[icut][3]->SetBinContent(htbin, metbin, bkgrate);
    	// Filling contour histograms and printing cuts for desired rate levels
    	for(int tag(0); tag < 3; tag++){
    	  hContour[icut][tag]->SetBinContent(htbin, metbin, hRate[0][icut][NSam-3+tag]->GetBinContent(htbin, metbin));
    	  if(iht < htlevels.size()){
    	    if(hContour[icut][tag]->GetXaxis()->GetBinLowEdge(htbin) >= htlevels[iht]){
    	      if(itag==tag && bkgrate<=levels[itag] || metbin==nBinsMet) {
    		if(itag==3) 
    		  cout<<"-- HT > "<<RoundNumber(hContour[icut][tag]->GetXaxis()->GetBinLowEdge(htbin),1)<<endl;
    		cout<<"MET > "<<RoundNumber(hContour[icut][tag]->GetYaxis()->GetBinLowEdge(metbin),1)
    		    <<": \t"<<RoundNumber(bkgrate,2)<<" Hz, \teff1200 "
    		    <<RoundNumber(hRate[0][icut][NSam-2]->GetBinContent(htbin, metbin),1)<<" %, \teff1500 "
    		    <<RoundNumber(hRate[0][icut][NSam-1]->GetBinContent(htbin, metbin),1)<<endl;
    		itag--;
    		if(itag==-1) {iht++; itag=3; cout<<endl;}
    	      }
    	    }
    	  }
    	} // Loop over contour lines
      } // Loop over MET bins
    } // Loop over HT bins

    leg.Clear();
    for(int tag(0); tag < 4; tag++){
      //hContour[icut][tag]->SetContour(1,&levels[tag]); hContour[icut][tag]->SetLineColor(lcolors[tag]);
      hContour[icut][tag]->SetLineWidth(4);
      hContour[icut][tag]->Draw(styles[tag]);
      leg.AddEntry(hContour[icut][tag], legtag[tag]);
    }
    // Plotting rates and efficiencies
    //Pnamebase = "plots/hltstudy/rate2d_"+tagfolder + "_"+Tags[icut]+ ".eps";
    Pnamebase = "plots/hltstudy/rate2d_"+Tags[icut]+ ".png";
    for(int sam(0); sam < NSam+1; sam++){
      if(Htag[sam].Contains("sig1200") ||Htag[sam].Contains("sig1500")) continue;
      Pname = Pnamebase; 
      if(Htag[sam].Contains("sig")) {
    	Pname.ReplaceAll("rate2d", Htag[sam]+"effi2d");
    	can.SetLogz(0);
      } else {
    	hRate[0][icut][sam]->SetMinimum(hRate[0][icut][2]->GetMinimum());
    	hRate[0][icut][sam]->SetMaximum(2*hRate[0][icut][NSam]->GetMaximum());
    	Pname.ReplaceAll("rate2d", Htag[sam]+"rate2d");
    	can.SetLogz(1);
      }
      hRate[0][icut][sam]->Draw("colz");
      hRate[1][icut][sam]->Draw("text same");
      // if(sam>=NSam-3) {
      // 	for(int tag(0); tag < 4; tag++) hContour[icut][tag]->Draw(styles[tag]);
      // 	leg.Draw();
      // }
      can.SaveAs(Pname);
    }
  } // Loop over cuts

  for(int icut(0); icut < NCuts; icut++){
    for(int tag(0); tag < NSam+1; tag++){
      for(int his(0); his < 2; his++)
  	if(hRate[his][icut][tag]) delete hRate[his][icut][tag];
    }
    for(int tag(0); tag < 4; tag++)
      if(hContour[icut][tag]) delete hContour[icut][tag];    
  }
}


void ReadChains(TChain *chain[], int entries[], TString folder){
  TString FileNames[] = 
    {folder+"/*T1tttt*1025_*", 
     folder+"/*T1tttt*1025_*", 
     folder+"/*T1tttt*1025_*", 
     folder+"/*QCD*",
     folder+"/*TT*",
     folder+"/W*"};

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
